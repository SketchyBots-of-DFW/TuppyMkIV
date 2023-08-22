FROM osrf/ros:humble-desktop-full

ARG DEBIAN_FRONTEND=noninteractive

SHELL [ "/bin/bash", "-c" ]

# use a closer mirror in dallas, texas
RUN cp /etc/apt/sources.list /etc/apt/sources.list.original && \
    sed -i -r 's,http://(.*).ubuntu.com,http://mirror.us-tx.kamatera.com,' /etc/apt/sources.list

# install basic dependnecies
RUN apt-get update && \
    apt-get -y --no-install-recommends install \
    mesa-utils \
    mesa-utils-extra \
    python3-pip \
    python-is-python3 \
    software-properties-common \
    wget

# downgrade setuptools to get rid of deprecated message and install python dependencies
RUN pip install setuptools==58.2.0 torch torchvision ultralytics

# Install Micro-XRCE-DDS Agent
RUN git clone https://github.com/eProsima/Micro-XRCE-DDS-Agent.git && \
    cd Micro-XRCE-DDS-Agent && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make && \
    make install

# create workspace folder and make it the starting folder
RUN mkdir -p /root/colcon_workspace && \
    echo "cd /root/colcon_workspace" >> /root/.bashrc

# source ROS setup.bash
RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc
