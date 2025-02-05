const express = require('express');
const path = require('path');
const yargs = require('yargs/yargs');
const { hideBin } = require('yargs/helpers');
const koffi = require('koffi');
require('expose-gc');

// Parse command line arguments
const argv = yargs(hideBin(process.argv))
    .usage('Usage: $0 [options]')
    .option('verbose', {
        alias: 'v',
        type: 'boolean',
        description: 'Run with verbose logging'
    })
    .option('port', {
        alias: 'p',
        type: 'number',
        description: 'Port to run the server on',
        default: 3000
    })
    .option('host', {
        type: 'string',
        description: 'Host to run the server on',
        default: 'localhost'
    })
    .help()
    .alias('help', 'h')
    .version()
    .argv;

const app = express();

app.use(express.json());
app.use(express.static('public'));

let nasalLib;
try {
    // First load the library
    const lib = koffi.load(path.join(__dirname, '../module/libnasal-web.dylib'));

    // Then declare the functions explicitly
    nasalLib = {
        nasal_init: lib.func('nasal_init', 'void*', []),
        nasal_cleanup: lib.func('nasal_cleanup', 'void', ['void*']),
        nasal_eval: lib.func('nasal_eval', 'const char*', ['void*', 'const char*', 'int']),
        nasal_get_error: lib.func('nasal_get_error', 'const char*', ['void*'])
    };

} catch (err) {
    console.error('Failed to load nasal library:', err);
    process.exit(1);
}

app.post('/eval', (req, res) => {
    const { code, showTime = false } = req.body;
    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    if (argv.verbose) {
        console.log('Received code evaluation request:', code);
        console.log('Show time:', showTime);
    }

    const ctx = nasalLib.nasal_init();
    try {
        const result = nasalLib.nasal_eval(ctx, code, showTime ? 1 : 0);
        const error = nasalLib.nasal_get_error(ctx);

        if (error && error !== 'null') {
            if (argv.verbose) console.log('Nasal error:', error);
            res.json({ error: error });
        } else if (result && result.trim() !== '') {
            if (argv.verbose) console.log('Nasal output:', result);
            res.json({ result: result });
        } else {
            if (argv.verbose) console.log('No output or error returned');
            res.json({ error: 'No output or error returned' });
        }
    } catch (err) {
        if (argv.verbose) console.error('Server error:', err);
        res.status(500).json({ error: err.message });
    } finally {
        if (argv.verbose) console.log('Cleaning up Nasal context');
        nasalLib.nasal_cleanup(ctx);
        global.gc()
    }
});

const PORT = argv.port || 3000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
    console.log(`Visit http://localhost:${PORT} to use the Nasal interpreter`);
    if (argv.verbose) console.log('Verbose logging enabled');
});
