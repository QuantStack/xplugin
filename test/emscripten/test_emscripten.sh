echo "TEST WITH EMSCRIPTEN"
# the directory of this script itself
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SOURCE_DIR="$DIR/../.."
SOURCE_DIR=$(readlink -f $SOURCE_DIR)


echo "SOURCE_DIR: $SOURCE_DIR"

BUILD_DIR="$SOURCE_DIR/build_emscripten"
# BUILD_DIR=$(readlink -f $BUILD_DIR)

echo "BUILD_DIR: $BUILD_DIR"

# create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR
echo $(pwd)

echo "CONFIGURE CMAKE WITH EMSCRIPTEN"
emcmake cmake  \
    -DCMAKE_BUILD_TYPE=Release \
    -DXPLUGIN_BUILD_TESTS=ON \
    -DXPLUGIN_BUILD_EXAMPLES=ON \
    $SOURCE_DIR

echo "MAKE WITH EMSCRIPTEN"
emmake make -j8

PLUGIN_DIR="$BUILD_DIR/test/testplugin_a"

if [ ! -f "$PLUGIN_DIR/libplugin_01.so" ]; then
    echo "ERROR: $PLUGIN_DIR/libplugin_01.so does not exist"
    exit 1
fi

if [ ! -f "$PLUGIN_DIR/libplugin_02.so" ]; then
    echo "ERROR: $PLUGIN_DIR/libplugin_02.so does not exist"
    exit 1
fi

if [ ! -f "$PLUGIN_DIR/libplugin_02.so" ]; then
    echo "ERROR: $PLUGIN_DIR/libplugin_02.so does not exist"
    exit 1
fi

if [ ! -f "$PLUGIN_DIR/libplugin_02.so" ]; then
    echo "ERROR: $PLUGIN_DIR/libplugin_02.so does not exist"
    exit 1
fi

if [ ! -f "$BUILD_DIR/test/xplugin_embind.js" ]; then
    echo "ERROR: $BUILD_DIR/test/xplugin_embind.js does not exist"
    exit 1
fi

if [ ! -f "$BUILD_DIR/test/xplugin_embind.wasm" ]; then
    echo "ERROR: $BUILD_DIR/test/xplugin_embind.wasm does not exist"
    exit 1
fi


# copy the test files to the build directory
mkdir -p $SOURCE_DIR/test/emscripten/content

cp $PLUGIN_DIR/libplugin_01.so $SOURCE_DIR/test/emscripten/content/
cp $PLUGIN_DIR/libplugin_02.so $SOURCE_DIR/test/emscripten/content/
cp $PLUGIN_DIR/libplugin_03.so $SOURCE_DIR/test/emscripten/content/
cp $BUILD_DIR/test/xplugin_embind.js $SOURCE_DIR/test/emscripten/content/
cp $BUILD_DIR/test/xplugin_embind.wasm $SOURCE_DIR/test/emscripten/content/


# run playwrite
python $SOURCE_DIR/test/emscripten/main_playwright.py

