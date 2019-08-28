# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "bento/ubuntu-18.04"
  config.vm.synced_folder ".", "/vagrant"
  config.ssh.forward_x11 = true
  config.ssh.forward_agent = true
  config.vm.provider "virtualbox" do |v|
    v.memory = 8192
    v.cpus = 4
  end

  clion = false
  if clion
    config.vm.post_up_message = "CLion workarounds enabled"

    # CLion relies on a fixed SSH address, so assign static SSH port here. This
    # must be unique on the host machine.
    port = 22000
    config.vm.post_up_message += "\nUse port #{port} for CLion remote host connection"
    config.vm.network "forwarded_port", id: "ssh", host: port, guest: 22, auto_correct: false

    # CLion only supports SFTP deployments for remote development, so disable
    # folder syncing and let CLion manage the project directory. This means
    # that the contents of the project directory cannot be relied upon while
    # provisioning the VM.
    remote = "/vagrant"
    config.vm.post_up_message += "\nUse #{remote} as the remote deployment directory"
    config.vm.synced_folder ".", remote, disabled: true
    config.vm.provision :shell, inline: "sudo sh -c '(mkdir #{remote} 2>/dev/null && chown vagrant:vagrant #{remote}) || true'"
  end

  # Work around disconnected virtual network cable.
  config.vm.provider "virtualbox" do |vb|
    vb.customize ["modifyvm", :id, "--cableconnected1", "on"]
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get -qqy update

    # Work around https://github.com/chef/bento/issues/661
    # apt-get -qqy upgrade
    DEBIAN_FRONTEND=noninteractive apt-get -y -o Dpkg::Options::="--force-confdef" -o Dpkg::Options::="--force-confold" upgrade

    apt-get -qqy install make zip unzip build-essential libcairo2-dev libgraphicsmagick1-dev libpng-dev xauth x11-apps

    cd /usr
    wget https://github.com/Kitware/CMake/releases/download/v3.15.2/cmake-3.15.2-Linux-x86_64.tar.gz
    tar -xvf cmake-3.15.2-Linux-x86_64.tar.gz
    cd cmake-3.15.2-Linux-x86_64
    cp -r bin/* ../bin
    cp -r doc/* ../doc
    cp -r man/* ../man
    cp -r share/* ../share
    cd ..
    rm -rf cmake-3.15.2-Linux-x86_64
    rm cmake-3.15.2-Linux-x86_64.tar.gz

    sudo apt-get -qqy install git
    sudo apt-get install -y g++ gdb gdbserver

    cd /opt
    git clone https://github.com/opencv/opencv.git
    cd opencv
    git checkout master
    cd ..

    git clone https://github.com/opencv/opencv_contrib.git
    cd opencv_contrib
    git checkout master
    cd ..

    cwd=$(pwd)

    echo "OpenCV installation by learnOpenCV.com"
    # Define OpenCV Version to install
    cvVersion="master"

    sudo apt -y remove x264 libx264-dev
    sudo apt -y install build-essential checkinstall pkg-config yasm
    sudo apt -y install git gfortran
    sudo apt -y install libjpeg8-dev libpng-dev
    sudo apt -y install software-properties-common
    sudo add-apt-repository "deb http://security.ubuntu.com/ubuntu xenial-security main"
    sudo apt -y update
    sudo apt -y install libjasper1
    sudo apt -y install libtiff-dev
    sudo apt -y install libavcodec-dev libavformat-dev libswscale-dev libdc1394-22-dev
    sudo apt -y install libxine2-dev libv4l-dev
    cd /usr/include/linux
    sudo ln -s -f ../libv4l1-videodev.h videodev.h
    cd "$cwd"

    sudo apt -y install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
    sudo apt -y install libgtk2.0-dev libtbb-dev qt5-default
    sudo apt -y install libatlas-base-dev
    sudo apt -y install libfaac-dev libmp3lame-dev libtheora-dev
    sudo apt -y install libvorbis-dev libxvidcore-dev
    sudo apt -y install libopencore-amrnb-dev libopencore-amrwb-dev
    sudo apt -y install libavresample-dev
    sudo apt -y install x264 v4l-utils

    sudo apt -y install libprotobuf-dev protobuf-compiler
    sudo apt -y install libgoogle-glog-dev libgflags-dev
    sudo apt -y install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

    sudo apt -y install python3-dev python3-pip
    sudo -H pip3 install -U pip numpy
    sudo apt -y install python3-testresources

    pip install wheel numpy scipy matplotlib scikit-image scikit-learn ipython dlib
    pip3 install wheel numpy scipy matplotlib scikit-image scikit-learn ipython dlib

    pip install opencv-python opencv-contrib-python
    pip3 install opencv-python opencv-contrib-python

    sudo apt-get install ffmpeg

    cd opencv
    mkdir build
    cd build
    cmake -D CMAKE_BUILD_TYPE=RELEASE \
                -D CMAKE_INSTALL_PREFIX=/usr/local \
                -D INSTALL_C_EXAMPLES=ON \
                -D INSTALL_PYTHON_EXAMPLES=ON \
                -D WITH_TBB=ON \
                -D WITH_V4L=ON \
                -D OPENCV_PYTHON3_INSTALL_PATH=/usr/local/lib/python3.5/site-packages \
            -D WITH_QT=ON \
            -D WITH_OPENGL=ON \
            -D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
            -D BUILD_EXAMPLES=ON ..
    make -j4
    make install
    ldconfig
    exit
    cd ~

    vagrantTip="[35m[1mThe shared directory is located at /vagrant\\nTo access your shared files: cd /vagrant[m"
    echo -e $vagrantTip > /etc/motd

    echo "Done installing your virtual machine!"
  SHELL
end
