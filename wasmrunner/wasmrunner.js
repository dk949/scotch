let inputFile;

const options = require("commander")
    .program
    .argument("<input-file>", "input wat file to be executed")
    .action(input => inputFile = input)
    .option("-e, --entry <function>", "Name of the exported funciton to be executed.", "main")
    .option("-o, --output-file <filename>", "output wasm file", "a.wasm")
    .option("-V, --verbose", "Print extra information on error")
    .option("--very-verbose", "Print extra information even on success")
    .description("Assemble .wat files and execute resulting wasm. Print output.")
    .version("0.1.0")
    .parse()
    .opts();

require("wabt")().then(wabt => {
    const fileContents = require("fs").readFileSync(inputFile, "utf8");

    const {buffer, log} = (wabt)
        .parseWat(inputFile, fileContents)
        .toBinary({});
    options.veryVerbose && console.log(log);
    options.veryVerbose && console.log(`Loaded ${inputFile} as binary`);
    options.veryVerbose && (console.log("file contents = \n   ", fileContents.toString().replace(/\n/g, "\n    ")));

    WebAssembly
        .compile(buffer)
        .then(b => {
            options.veryVerbose && console.log("successfully compiled the binary");
            const instance = WebAssembly.instantiate(b);
            options.veryVerbose && console.log("successfully created wasm instance");
            return instance;
        })
        .then(e => console.log(e.exports[options.entry]()))
        .catch(err => {
            console.error(err);
            (options.verbose || options.veryVerbose) && console.error(log);
            process.exit(1)
        });
    options.veryVerbose && console.log(log);
}).catch(err => {
    console.error(err);
    process.exit(1);
});
