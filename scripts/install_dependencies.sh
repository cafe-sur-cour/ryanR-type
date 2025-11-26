if [ -z "vcpkg/" ] || [ -z "vcpkg/bootstrap-vcpkg.sh" ]; then
    rm -rf vcpkg
    git clone https://github.com/Microsoft/vcpkg.git
fi

cd vcpkg

./bootstrap-vcpkg.sh

export VCPKG_ROOT=$(pwd)
export PATH="$PATH:$VCPKG_ROOT"

echo "VCPKG_ROOT set to $VCPKG_ROOT"
