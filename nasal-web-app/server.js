const express = require('express');
const ffi = require('ffi-napi');
const path = require('path');

const app = express();

app.use(express.json());
app.use(express.static('public'));

const nasalLib = ffi.Library(path.join(__dirname, '../module/libnasal-web'), {
    'nasal_init': ['pointer', []],
    'nasal_cleanup': ['void', ['pointer']],
    'nasal_eval': ['string', ['pointer', 'string']],
    'nasal_get_error': ['string', ['pointer']]
});

app.post('/eval', (req, res) => {
    const { code } = req.body;
    if (!code) {
        return res.status(400).json({ error: 'No code provided' });
    }

    const ctx = nasalLib.nasal_init();
    try {
        const result = nasalLib.nasal_eval(ctx, code);
        const error = nasalLib.nasal_get_error(ctx);
        
        // Check if there's an error first
        if (error && error !== 'null' && error.trim() !== '') {
            console.log('Nasal error:', error); // For debugging
            res.json({ error: error });
        } else if (result && result.trim() !== '') {
            console.log('Nasal output:', result); // For debugging
            res.json({ result: result });
        } else {
            res.json({ error: 'No output or error returned' });
        }
    } catch (err) {
        console.error('Server error:', err); // For debugging
        res.status(500).json({ error: err.message });
    } finally {
        nasalLib.nasal_cleanup(ctx);
    }
});

const PORT = process.env.PORT || 3000;
app.listen(PORT, () => {
    console.log(`Server running on port ${PORT}`);
    console.log(`Visit http://localhost:${PORT} to use the Nasal interpreter`);
}); 