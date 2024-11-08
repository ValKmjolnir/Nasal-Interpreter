const express = require('express');
const path = require('path');
const yargs = require('yargs/yargs');
const { hideBin } = require('yargs/helpers');
const koffi = require('koffi');

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
        default: 3001
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


// Load Nasal REPL library functions
let nasalLib;
try {
    const lib = koffi.load(path.join(__dirname, '../module/libnasal-web.dylib'));
    
    nasalLib = {
        nasal_repl_init: lib.func('nasal_repl_init', 'void*', []),
        nasal_repl_cleanup: lib.func('nasal_repl_cleanup', 'void', ['void*']),
        nasal_repl_eval: lib.func('nasal_repl_eval', 'const char*', ['void*', 'const char*']),
        nasal_repl_is_complete: lib.func('nasal_repl_is_complete', 'int', ['void*', 'const char*']),
        nasal_repl_get_version: lib.func('nasal_repl_get_version', 'const char*', [])
    };

    if (argv.verbose) {
        console.log('REPL Library loaded successfully');
    }
} catch (err) {
    console.error('Failed to load REPL library:', err);
    process.exit(1);
}

// Store active REPL sessions
const replSessions = new Map();

// Clean up inactive sessions periodically (30 minutes timeout)
const SESSION_TIMEOUT = 30 * 60 * 1000;
setInterval(() => {
    const now = Date.now();
    for (const [sessionId, session] of replSessions.entries()) {
        if (now - session.lastAccess > SESSION_TIMEOUT) {
            if (argv.verbose) {
                console.log(`Cleaning up inactive session: ${sessionId}`);
            }
            nasalLib.nasal_repl_cleanup(session.context);
            replSessions.delete(sessionId);
        }
    }
}, 60000); // Check every minute

app.post('/repl/init', (req, res) => {
    try {
        const ctx = nasalLib.nasal_repl_init();
        const sessionId = `session_${Date.now()}_${Math.random().toString(36).substr(2, 9)}`;
        const version = nasalLib.nasal_repl_get_version();
        
        replSessions.set(sessionId, {
            context: ctx,
            lastAccess: Date.now()
        });

        if (argv.verbose) {
            console.log(`New REPL session initialized: ${sessionId}`);
        }

        res.json({ 
            sessionId,
            version
        });
    } catch (err) {
        if (argv.verbose) {
            console.error('Failed to initialize REPL session:', err);
        }
        res.status(500).json({ error: 'Failed to initialize REPL session' });
    }
});

app.post('/repl/eval', (req, res) => {
    const { sessionId, line, check, buffer } = req.body;
    
    if (!sessionId || !replSessions.has(sessionId)) {
        return res.status(400).json({ error: 'Invalid or expired session' });
    }

    if (!line) {
        return res.status(400).json({ error: 'No code provided' });
    }

    try {
        const session = replSessions.get(sessionId);
        session.lastAccess = Date.now();

        if (check) {
            const codeToCheck = buffer ? [...buffer, line].join('\n') : line;
            const isComplete = nasalLib.nasal_repl_is_complete(session.context, codeToCheck);
            
            if (isComplete === 1) {
                return res.json({ needsMore: true });
            } else if (isComplete === -1) {
                return res.json({ error: 'Invalid input' });
            }
        }

        const result = nasalLib.nasal_repl_eval(session.context, line);
        
        if (argv.verbose) {
            console.log(`REPL evaluation for session ${sessionId}:`, { line, result });
        }

        res.json({ result });
    } catch (err) {
        if (argv.verbose) {
            console.error(`REPL evaluation error for session ${sessionId}:`, err);
        }
        res.status(500).json({ error: err.message });
    }
});

app.post('/repl/cleanup', (req, res) => {
    const { sessionId } = req.body;
    
    if (!sessionId || !replSessions.has(sessionId)) {
        return res.status(400).json({ error: 'Invalid session' });
    }

    try {
        const session = replSessions.get(sessionId);
        nasalLib.nasal_repl_cleanup(session.context);
        replSessions.delete(sessionId);

        if (argv.verbose) {
            console.log(`REPL session cleaned up: ${sessionId}`);
        }

        res.json({ message: 'Session cleaned up successfully' });
    } catch (err) {
        if (argv.verbose) {
            console.error(`Failed to cleanup session ${sessionId}:`, err);
        }
        res.status(500).json({ error: err.message });
    }
});

// Handle cleanup on server shutdown
process.on('SIGINT', () => {
    console.log('\nCleaning up REPL sessions before exit...');
    for (const [sessionId, session] of replSessions.entries()) {
        try {
            nasalLib.nasal_repl_cleanup(session.context);
            if (argv.verbose) {
                console.log(`Cleaned up session: ${sessionId}`);
            }
        } catch (err) {
            console.error(`Error cleaning up session ${sessionId}:`, err);
        }
    }
    process.exit(0);
});

const PORT = argv.port || 3001;
app.listen(PORT, () => {
    console.log(`REPL Server running on http://localhost:${PORT}`);
    if (argv.verbose) console.log('Verbose logging enabled');
}); 