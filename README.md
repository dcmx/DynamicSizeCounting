# DynamicSizeCounting
Simulator Software for our Dynamic Size Counting Population Protocol

In this repository we will provide our simulator software for our paper 
Dominik Kaaser and Maximilian Lohmann. 2024. Dynamic Size Counting in the Population Protocol Model: Counting (on) agents to drive a phase clock. In ACM Symposium on Principles of Distributed Computing (PODC ’24), June 17–21, 2024, Nantes, France. ACM, New York, NY, USA, 11 pages. https://doi.org/10.1145/3662158.3662825

## Requirements
This project was tested using CMake 3.26.0, clang 15.0.0, and python3.10 on MacOS.
Specific python package requirements are listed in the `requirements.txt` file.

## Usage
The simulator is written in C++ and is built with CMake.
To run the build and the simulator, you can run the `simulate.sh` script in the `sim` directory.
This writes the results as CSV files into the `outputs` directory.

The analysis script is written in a Jupyter Notebook.
To install the necessary dependencies, you need to have Python and pip installed.
Then you can run `pip install -r requirements.txt`.
To start the notebook, navigate to the `analysis` directory, run `jupyter notebook`, and select the `analysis.ipynb` file.
This reads the outputs of the simulator and writes the resulting plots into the `results` subdirectory.