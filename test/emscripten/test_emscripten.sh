echo "TEST WITH EMSCRIPTEN"
# the directory of this script itself
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SOURCE_DIR="$DIR/../.."
SOURCE_DIR=$(readlink -f $SOURCE_DIR)


echo "SOURCE_DIR: $SOURCE_DIR"

BUILD_DIR="$SOURCE_DIR/../build_emscripten"
BUILD_DIR=$(readlink -f $BUILD_DIR)

# create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR
echo $(pwd)

echo "CONFIGURE CMAKE WITH EMSCRIPTEN"
emcmake cmake  \
    -DCMAKE_BUILD_TYPE=Release \
    $SOURCE_DIR

echo "MAKE WITH EMSCRIPTEN"
emmake make -j8

