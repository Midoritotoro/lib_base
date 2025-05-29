#!/bin/bash

> CMakeBuildOptions.txt
check_python_installation() {
    echo "Checking for Python 3.11 existence..."
    if python --version >/dev/null 2>&1; then
        echo "Python was found."
        start_packages_installation
    else
        echo ""
        echo "Error: Python not installed."
        echo "Attempting to install python..."
        try_to_install_python
    fi
}

try_to_install_python() {
    echo "Downloading Python installer..."
    
    curl -s -L "https://www.python.org/ftp/python/3.11.0/python-3.11.0-amd64.exe" -o "/tmp/python-3.11.0-amd64.exe"

    if [ $? -ne 0 ]; then
        echo "Error downloading Python installer."
        return
    fi
    
    echo "Installing python..."

    sudo apt install python3.11 -y || {
      echo "Python install failed."
        return 1
    }

    export PATH=$PATH:/opt/python311/bin
    echo "Python installation successful."

    start_packages_installation
}

start_packages_installation() {
    echo "Installing packages..."
    python DependencyInstaller.py
}

process_arguments() {
    while [ $# -gt 0 ]; do
        echo "$1" >> CMakeBuildOptions.txt
        shift
    done
}

process_arguments "$@"
check_python_installation