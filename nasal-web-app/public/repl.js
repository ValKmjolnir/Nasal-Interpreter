let replSessionId = null;
let multilineInput = [];
let historyIndex = -1;
let commandHistory = [];
let multilineBuffer = [];
let isMultilineMode = false;

// Initialize REPL
async function initRepl() {
    try {
        const response = await fetch('/repl/init', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        });
        const data = await response.json();
        if (data.sessionId) {
            replSessionId = data.sessionId;
            console.log('REPL session initialized:', replSessionId);
            
            // Display version info
            appendOutput('Nasal REPL interpreter version ' + data.version);
            appendOutput('Type your code below and press Enter to execute.');
            appendOutput('Use Shift+Enter for multiline input.\n');
            showPrompt();
        } else {
            throw new Error('Failed to initialize REPL session');
        }
    } catch (err) {
        appendOutput(`Error: ${err.message}`, 'error-line');
    }
}

// Format error messages to match command-line REPL
function formatError(error) {
    // Split the error message into lines
    const lines = error.split('\n');
    return lines.map(line => {
        // Add appropriate indentation for the error pointer
        if (line.includes('-->')) {
            return '  ' + line;
        } else if (line.includes('^')) {
            return '  ' + line;
        }
        return line;
    }).join('\n');
}

// Handle input
const input = document.getElementById('repl-input');
input.addEventListener('keydown', async (e) => {
    if (e.key === 'Enter') {
        if (e.shiftKey) {
            // Shift+Enter: add newline
            const pos = input.selectionStart;
            const value = input.value;
            input.value = value.substring(0, pos) + '\n' + value.substring(pos);
            input.selectionStart = input.selectionEnd = pos + 1;
            input.style.height = 'auto';
            input.style.height = input.scrollHeight + 'px';
            e.preventDefault();
            return;
        }
        e.preventDefault();
        
        const code = input.value.trim();
        
        // Skip empty lines but still show prompt
        if (!code) {
            showPrompt(isMultilineMode ? '... ' : '>>> ');
            return;
        }

        try {
            // First check if input is complete
            const checkResponse = await fetch('/repl/eval', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    sessionId: replSessionId,
                    line: code,
                    check: true,
                    buffer: multilineBuffer // Send existing buffer
                })
            });

            const checkData = await checkResponse.json();
            
            if (checkData.needsMore) {
                // Add to multiline buffer and show continuation prompt
                multilineBuffer.push(code);
                isMultilineMode = true;
                
                // Display the input with continuation prompt
                appendOutput(code, 'input-line', multilineBuffer.length === 1 ? '>>> ' : '... ');
                
                input.value = '';
                showPrompt('... ');
                return;
            }

            // If we were in multiline mode, add the final line
            if (isMultilineMode) {
                multilineBuffer.push(code);
            }

            // Get the complete code to evaluate
            const fullCode = isMultilineMode ? 
                multilineBuffer.join('\n') : code;

            // Display the input
            appendOutput(code, 'input-line', isMultilineMode ? '... ' : '>>> ');

            // Evaluate the code
            const response = await fetch('/repl/eval', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({
                    sessionId: replSessionId,
                    line: fullCode
                })
            });

            const data = await response.json();
            
            if (data.error) {
                appendOutput(formatError(data.error.trim()), 'error-line');
            } else if (data.result) {
                handleResult(data.result);
            }

            // Reset multiline state
            multilineBuffer = [];
            isMultilineMode = false;
            input.value = '';
            showPrompt('>>> ');

        } catch (err) {
            appendOutput(`Error: ${err.message}`, 'error-line');
            multilineBuffer = [];
            isMultilineMode = false;
            input.value = '';
            showPrompt('>>> ');
        }
    } else if (e.key === 'ArrowUp') {
        e.preventDefault();
        if (historyIndex > 0) {
            historyIndex--;
            input.value = commandHistory[historyIndex];
        }
    } else if (e.key === 'ArrowDown') {
        e.preventDefault();
        if (historyIndex < commandHistory.length - 1) {
            historyIndex++;
            input.value = commandHistory[historyIndex];
        } else {
            historyIndex = commandHistory.length;
            input.value = '';
        }
    }
});

// Auto-resize input
input.addEventListener('input', () => {
    input.style.height = 'auto';
    input.style.height = input.scrollHeight + 'px';
});

// Show prompt and scroll to bottom
function showPrompt(prompt = '>>> ') {
    const promptSpan = document.querySelector('.repl-prompt');
    if (promptSpan) {
        promptSpan.textContent = prompt;
    }
}

// Append output to REPL
function appendOutput(text, className = '', prefix = '') {
    const output = document.getElementById('repl-output');
    const line = document.createElement('div');
    line.className = `output-line ${className}`;

    line.innerHTML = prefix + formatErrorMessage(text);
   
    
    output.appendChild(line);
    output.scrollTop = output.scrollHeight;
}

// Clear REPL
function clearREPL() {
    const output = document.getElementById('repl-output');
    output.innerHTML = '';
    appendOutput('Screen cleared', 'system-message');
    showPrompt();
}

// Example snippets
const examples = {
    basic: `var x = 1011 + 1013;
println("x = ", x);`,
    
    loops: `var sum = 0;
for(var i = 1; i <= 5; i += 1) {
    sum += i;
}
println("Sum:", sum);`,
    
    functions: `var factorial = func(n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
};
factorial(5);`
};

// Insert example into input
function insertExample(type) {
    input.value = examples[type];
    input.style.height = 'auto';
    input.style.height = input.scrollHeight + 'px';
    input.focus();
}

// Initialize REPL on page load
window.addEventListener('load', initRepl);

// Add these utility functions
function formatErrorMessage(text) {
    // Replace ANSI escape codes with CSS classes
    return text
        // Remove any existing formatting first
        .replace(/\u001b\[\d+(?:;\d+)*m/g, '')
        // Format the error line
        .replace(/^parse: (.+)$/m, '<span class="error-type">parse:</span> <span class="error-desc">$1</span>')
        // Format the file location
        .replace(/^\s*--> (.+?)(\d+):(\d+)$/m, '<span class="error-arrow">--></span> <span class="error-file">$1</span><span class="error-loc">$2:$3</span>')
        // Format the code line
        .replace(/^(\d+ \|)(.*)$/m, '<span class="error-line-number">$1</span><span class="error-code">$2</span>')
        // Format the error pointer
        .replace(/^(\s*\|)(\s*)(\^+)$/m, '<span class="error-line-number">$1</span><span class="error-pointer-space">$2</span><span class="error-pointer">$3</span>');
}

function handleResult(result) {
    const lines = result.split('\n');
    lines.forEach(line => {
        if (line.trim()) {
            appendOutput(line.trim(), 'result-line');
        }
    });
}