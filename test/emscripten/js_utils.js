async () =>{
    function createDynlibFS(
        lib,
        searchDirs,
        readFileFunc,
        Module
    ) {
        let _searchDirs = searchDirs || [];
        const resolvePath = (path) => {
            for (const dir of _searchDirs) {
                const fullPath = Module.PATH.join2(dir, path);
                if (Module.FS.findObject(fullPath) !== null) {
                    return fullPath;
                }
            }
            return path;
        };

        let readFile = (path) =>
            Module.FS.readFile(resolvePath(path));

        if (readFileFunc !== undefined) {
            readFile = (path) => readFileFunc(resolvePath(path));
        }

        const fs = {
            findObject: (path, dontResolveLastLink) => {
                let obj = Module.FS.findObject(resolvePath(path), dontResolveLastLink);

                if (obj === null) {
                    console.debug(`Failed to find a library: ${resolvePath(path)}`);
                }

                return obj;
            },
            readFile: readFile,
        };

        return fs;
    }


    // main function
    let Module = await createModule();
    globalThis.Module = Module;
    

    let plugin_names = [
        "plugin_01",
        "plugin_02",
        "plugin_03",
    ]

    // make dir
    Module.FS.mkdir("/plugins");

    // fetch all plugins
    for (let i = 0; i < plugin_names.length; i++) {
        let plugin_name = plugin_names[i];
        let response = await fetch(`./lib${plugin_name}.so`);
        // console.log("response", response);
        let buffer = await response.arrayBuffer();

        // write to emscripten fs
        let filename = `/plugins/lib${plugin_name}.so`;
        let data = new Uint8Array(buffer);

        Module.FS.writeFile(filename, data);

        // instantiate all SO files
        const fs = createDynlibFS(filename, ["/plugins/"], Module.FS.readFile, Module);
        const libName = Module.PATH.basename(filename);

        await Module.loadDynamicLibrary(libName, {
            loadAsync: true,
            nodelete: true,
            allowUndefined: true,
            global: true,
            fs: fs
         })
    }
    let ret = Module.run_tests();
    return ret;
        
}