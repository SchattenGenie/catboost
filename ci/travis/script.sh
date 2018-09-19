#!/bin/bash -ex

if [ "$TRAVIS_OS_NAME" = 'osx' ]; then
    export PYTHONPATH=$PYTHONPATH:/usr/local/lib/python2.7/site-packages
fi

function install_cuda_linux()
{
    CUDA_REPO_PKG="cuda-repo-ubuntu1604-9-0-local_9.0.176-1_amd64-deb"
    wget https://developer.nvidia.com/compute/cuda/9.0/Prod/local_installers/${CUDA_REPO_PKG}
    sudo dpkg -i ${CUDA_REPO_PKG}
    sudo apt-key adv --fetch-keys http://developer.download.nvidia.com/compute/cuda/repos/ubuntu1604/x86_64/7fa2af80.pub
    sudo apt-get update
    sudo apt-get -y install nvidia-settings cuda-drivers cuda-runtime-9-0 cuda-demo-suite-9-0 cuda-9-0
    export PATH=${PATH}:/usr/local/cuda-9.0/bin
    export CUDA_HOME=${CUDA_HOME}:/usr/local/cuda:/usr/local/cuda-9.0
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/cuda-9.0/lib64
}


if [ "${CB_BUILD_AGENT}" == 'clang-linux-x86_64-release-cuda' ]; then
    install_cuda_linux;
    echo $CXX
    echo $CC
    ./ya make --no-emit-status --stat -T -r -j 1 catboost/app -DCUDA_ROOT=/usr/local/cuda-9.0 -DNO_DEBUGINFO;
    cp $(readlink -f catboost/app/catboost) catboost-cuda-linux;
    python ci/webdav_upload.py catboost-cuda-linux
fi

if [ "${CB_BUILD_AGENT}" == 'python2-linux-x86_64-release' ]; then
     install_cuda_linux;
     cd catboost/python-package;
     python2 ./mk_wheel.py --no-emit-status -T -j 1 -DCUDA_ROOT=/usr/local/cuda-9.0 ;
     python ../../ci/webdav_upload.py *.whl
fi

if [ "${CB_BUILD_AGENT}" == 'python34-linux-x86_64-release' ]; then
     ls /home/travis/virtualenv
     ln -s /home/travis/virtualenv/python3.4.6/bin/python-config /home/travis/virtualenv/python3.4.6/bin/python3-config;
     # install_cuda_linux;
     cd catboost/python-package;
     python3 ./mk_wheel.py --no-emit-status -T -j 1 -DPYTHON_CONFIG=/home/travis/virtualenv/python3.4.6/bin/python3-config;
     python ../../ci/webdav_upload.py *.whl
fi

if [ "${CB_BUILD_AGENT}" == 'python35-linux-x86_64-release' ]; then
     ls /home/travis/virtualenv
     ln -s /home/travis/virtualenv/python3.5.6/bin/python-config /home/travis/virtualenv/python3.5.6/bin/python3-config;
     install_cuda_linux;
     cd catboost/python-package;
     python3 ./mk_wheel.py --no-emit-status -T -j 1 -DCUDA_ROOT=/usr/local/cuda-9.0 -DPYTHON_CONFIG=/home/travis/virtualenv/python3.5.6/bin/python3-config;
     python ../../ci/webdav_upload.py *.whl
fi

if [ "${CB_BUILD_AGENT}" == 'python36-linux-x86_64-release' ]; then
     ln -s /home/travis/virtualenv/python3.6.3/bin/python-config /home/travis/virtualenv/python3.6.3/bin/python3-config;
     install_cuda_linux;
     cd catboost/python-package;
     python3 ./mk_wheel.py --no-emit-status -T -j 1 -DCUDA_ROOT=/usr/local/cuda-9.0 -DPYTHON_CONFIG=/home/travis/virtualenv/python3.6.3/bin/python3-config;
     python ../../ci/webdav_upload.py *.whl
fi

if [ "${CB_BUILD_AGENT}" == 'clang-darwin-x86_64-release' ]; then
    ./ya make --no-emit-status --stat -T -r -j 1 catboost/app;
    cp $(readlink catboost/app/catboost) catboost-darwin;
    python ci/webdav_upload.py catboost-darwin
fi

if [ "${CB_BUILD_AGENT}" == 'R-clang-darwin-x86_64-release' ]; then
    cd catboost/R-package

    mkdir catboost

    cp DESCRIPTION catboost
    cp NAMESPACE catboost
    cp README.md catboost

    cp -r R catboost

    cp -r inst catboost
    cp -r man catboost
    cp -r tests catboost

    ../../ya make -r -T src

    mkdir catboost/inst/libs
    cp $(readlink src/libcatboostr.so) catboost/inst/libs

    tar -cvzf catboost-R-$(uname).tgz catboost
    python ../../ci/webdav_upload.py catboost-R-*.tgz
fi


if [ "${CB_BUILD_AGENT}" == 'R-clang-linux-x86_64-release' ]; then
    cd catboost/R-package

    mkdir catboost

    cp DESCRIPTION catboost
    cp NAMESPACE catboost
    cp README.md catboost

    cp -r R catboost

    cp -r inst catboost
    cp -r man catboost
    cp -r tests catboost

    ../../ya make -r -T src

    mkdir catboost/inst/libs
    cp $(readlink src/libcatboostr.so) catboost/inst/libs

    tar -cvzf catboost-R-$(uname).tgz catboost
    python ../../ci/webdav_upload.py catboost-R-*.tgz
fi
