README - VIO: Visual-Inertial Odometry
======================================

What is VIO?
------------

VIO is a library of C++ classes that implement the visual-inertial odometry pipeline described in these papers:
 
 - C. Forster, L. Carlone, F. Dellaert, and D. Scaramuzza. On-Manifold Preintegration Theory for Fast and Accurate Visual-Inertial Navigation. IEEE Trans. Robotics, 33(1):1-21, 2016.
 - C. Forster, L. Carlone, F. Dellaert, and D. Scaramuzza. On-Manifold Preintegration Theory for Fast and Accurate Visual-Inertial Navigation. IEEE Trans. Robotics, 33(1):1-21, 2016.
 - L. Carlone, Z. Kira, C. Beall, V. Indelman, and F. Dellaert. Eliminating Conditionally Independent Sets in Factor Graphs: A Unifying Perspective based on Smart Factors. In IEEE Intl. Conf. on Robotics and Automation (ICRA), 2014.

Quickstart
----------

Clone this repository: `git clone git@github.mit.edu:SPARK/VIO.git`

In the root library folder execute (using cmake-gui: if you changed the GTSAM install folder, you may need to redirect VIO to your-gtsam-install-folder/lib/cmake/GTSAM. Similarly,you may need to change the folder for CGAL and OpenGV):

```
#!bash
$ mkdir build
$ cd build
$ cmake ../
$ make
$ make check
```

Note 1a: if you use MKL in gtsam, you may need to add to .bashrc a line similar to: source /opt/intel/parallel_studio_xe_2018/compilers_and_libraries_2018/linux/mkl/bin/mklvars.sh intel64
(Alternatively, type `locate compilervars.sh` and then `source $output file.sh that you got from locate$`, add to your .bashrc to automate).
Note 1b: sometimes you may need to add /usr/local/lib to LD_LIBRARY_PATH in ~/.bashrc (if you get lib not found errors at run or test time)
Note 2: you may have to add %YAML:1.0 as first line in all YAML files :-(
Note 3: we are considering to enable EPI in GTSAM, which will require to set the GTSAM_THROW_CHEIRALITY_EXCEPTION to false (cmake flag).
Note 4: for better performance when using the IMU factors, set GTSAM_TANGENT_PREINTEGRATION to false (cmake flag) 

Prerequisites:

- [GTSAM](https://bitbucket.org/gtborg/gtsam/overview/) >= 4.0 (Branch: `feature/ImprovementsIncrementalFilter`, commit: `c827d4cd6b11f78f3d2d9d52b335ac562a2757fc`)
- [OpenCV](https://opencv.org/opencv-3-0.html) >= 3.0 (Installation instructions below)
- [OpenGV] Installation instructions below 
- [CGAL] Installation instructions below

Installation of OpenCV
----------------------
- on Mac:
```
#!bash
$ homebrew install vtk (to check)
download opencv3.3.1 from https://opencv.org/releases.html
unzip and go to opencv3.3.1
$ mkdir build
$ cd build
$ cmake ../
$ sudo make install
```
- on Linux:
```
#!bash
$ sudo apt-get install libvtk5-dev   (libvtk6-dev in ubuntu 17.10)
$ sudo apt-get install libgtk2.0-dev 
$ sudo apt-get install pkg-config
download opencv3.3.1 from https://opencv.org/releases.html
unzip and go to opencv3.3.1
$ mkdir build
$ cd build
$ cmake -DWITH_VTK=On ..
$ sudo make -j8 install
$ sudo make -j8 test (optional - quite slow)
```

Installation of OpenGV
----------------------
- git clone https://github.com/laurentkneip/opengv (I did this in my "home/code/" folder)
- (not needed in latest version) open CMakeLists.txt and set INSTALL_OPENGV to ON (this can be also done using cmake-gui)
- using cmake-gui, set: the eigen version to the GTSAM one (for me: /Users/Luca/borg/gtsam/gtsam/3rdparty/Eigen). if you don't do so, very weird error appear (may be due to GTSAM and OpenGV using different versions of eigen!)
- in the opengv folder do:

```
#!bash
$ mkdir build
$ cd build
$ cmake ../
$ sudo make -j8 install
$ sudo make -j8 check
```

Installation of CGAL
----------------------
- download CGAL `https://www.cgal.org/download.html` (I tried CGAL-4.11 on Ubuntu 17.10)
- go to CGAL downloaded folder and execute the following:

```
#!bash
$ mkdir build
$ cd build
$ cmake ../
```
- using cmake-gui enable WithEigen3, click configure, and set the eigen version to the GTSAM one (for me: /Users/Luca/borg/gtsam/gtsam/3rdparty/Eigen)
- using cmake-gui check that CMAKE_BUILD_TYPE is set to 'Release" 
- go back to the build folder and execute the following:

```
#!bash
$ make -j8 
$ sudo make install
```

Glog and Gflags
----------------------
Glog and gflags will be automatically downloaded using cmake unless there is a system-wide installation found.

Running examples
======================

### Euroc Dataset

stereoVIOEuroc:
- Download [EuRoC](http://projects.asl.ethz.ch/datasets/doku.php?id=kmavvisualinertialdatasets) dataset. You can just download this [file](http://robotics.ethz.ch/~asl-datasets/ijrr_euroc_mav_dataset/machine_hall/MH_01_easy/MH_01_easy.zip) to do a first test, which corresponds to the ```MH_01_easy``` EuRoC dataset.
- Add the comment ```%YAML:1.0``` at the top of each .yaml file in the dataset (each folder has one sensor.yaml). You can do this manually or alternatively paste and run the following bash script from within the dataset folder:
```
#!bash
sed -i '1 i\%YAML:1.0' body.yaml
sed -i '1 i\%YAML:1.0' */sensor.yaml
```
You have two ways to start the example:
- Using the script ```stereoVIOEuroc.bash``` inside the ```scripts``` folder:
  - Run: ```./stereoVIOEuroc.bash -p "PATH_TO_DATASET/MH_01_easy"```
  - Optionally, you can try the VIO using structural regularities, as in [Toni's thesis](https://www.research-collection.ethz.ch/handle/20.500.11850/297645), by specifying the option ```-r```: ```./stereoVIOEuroc.bash -p "PATH_TO_DATASET/MH_01_easy" -r```
- Alternatively, have a look inside the script to see how to change extra parameters that control the pipeline itself.

### Kitti dataset

- Download raw data from [Kitti ](http://www.cvlibs.net/datasets/kitti/raw_data.php?type=residential) (in order to have IMU messages). In particular:
  - Download unsynced + unrectified raw dataset (e.g. [2011\_09\_26\_drive\_0039\_extract](https://s3.eu-central-1.amazonaws.com/avg-kitti/raw_data/2011_09_26_drive_0039/2011_09_26_drive_0039_extract.zip)).
  - Download as well the calibration data (three files) from raw dataset (e.g. [2011\_09\_26\_calib](https://s3.eu-central-1.amazonaws.com/avg-kitti/raw_data/2011_09_26_calib.zip)), and save them inside the folder `2011_09_26_drive_0039_extract`.

- Run: `./scripts/stereoVIOKitti.bash -p PATH_TO_DATASET` where you specify the path to the dataset (e.g. path to '2011_09_26_drive_0039_extract' folder).


Tips for usage
----------------------
- The 3D Visualization window implements the following keyboard shortcuts:
    - Press 't': toggle freezing visualization (as of know, this blocks the whole pipeline, it might change once the visualization is threaded).
    - Press 'v': prints to the terminal the pose of the current viewpoint of the 3D visualization window.
    - Press 'w': prints to the terminal the size of the 3D visualization window.

     These last two shortcuts are useful if you want to programmatically set the initial viewpoint and size of the screen when launching the 3D visualization window (this is done at the constructor of the 3DVisualizer class).
    - Press 's': to get a screenshot of the 3D visualization window.
    - Press '0', '1', or '2': to toggle the 3D mesh representation (only visible if the gflag 'visualize_mesh' is set to true).
    - Press 'a': to toggle ambient light for the 3D mesh ('visualize_mesh' has to be set to true).
    - Press 'l': to toggle lighting for the 3D mesh ('visualize_mesh' has to be set to true).

> For these shortcuts to take effect you need to have the window in focus. Note also that there might be a big delay between your keypress and the actual processing of the information as this is purely sequential with the VIO pipeline itself; this might change after parallelizing the pipeline).

- The 3D Visualization allows to load an initial `.ply` file.
This is useful for example for the Euroc dataset `V1_01_*` where we are given a point-cloud of the scene. Note that the `.ply` file must have the following properties in the header, and its corresponding entries:
  - A vertex element: `element vertex 3199068` (actual number depends on how many vertices has the ply file).
  - A face element: `element face 0`(actual number depends on how many faces has the ply file)
  - Properties may vary, as for now, any given `.ply` file will be displayed as a point-cloud, not a mesh (due to some weirdness in opencv).
  - For example, the ground-truth point-cloud provided in `V1_01_easy` dataset must be modified to have the following header:
      ```
      ply
      format ascii 1.0
      element vertex 3199068
      property float x
      property float y
      property float z
      property float intensity
      property uchar diffuse_red
      property uchar diffuse_green
      property uchar diffuse_blue
      element face 0
      property list uint8 int32 vertex_indices
      end_header
      ```

Tips for development
----------------------
- To make the pipeline deterministic:
    - Disable TBB in GTSAM (go to build folder in gtsam, use cmake-gui to unset ```GTSAM_WITH_TBB```).
    - Specify ```srand(0)``` in main function, to make randomized algorithms deterministic.
    - Change ```ransac_randomize``` flag in ```params/trackerParameters.yaml``` to 0, to disable ransac randomization.

> Note: these changes are not sufficient to make the output repeatable between different machines.

Tips for speed
----------------------

- Use ```-march=native``` compiler flag. It will be a problem if you ever want to distribute binaries of this code.
- GTSAM speed depends on: TBB, OpenMP. In principle, TBB speeds-up things, OpenMP not so sure. Verify that on your own computer.
- Make sure OpenCV does not have the `CV_TRACE` cmake flag set-up. This is for profiling. Also use `TBB` as well. Compile with `Lapack`, it seems that operations such as SVD are faster.
- OpenCV could potentially be used with GPU: stereo visual odometry achieves a 7x speed-up.
- Make sure all libraries are compiled with the maximum optimization. Set `CMAKE_BUILD_TYPE` to `Release` and ensure you get a level of optimization at `-O3` for best results.

# Use code linter

To contribute to this repo, ensure your commits pass the linter pre-commit checks.

## Dependencies

Install the following dependencies to run the linter:

 * **pylint**
   * macOS:
     ```
     pip install pylint
     ```
 * **yapf**
   * Ubuntu / macOS: `pip install yapf`
 * **clang-format**
   * Compatible with `clang-format-3.8 - 6.0`
   * Ubuntu: `sudo apt install clang-format-${VERSION}`
   * macOS:
     ```
     brew install clang-format
     ln -s /usr/local/share/clang/clang-format-diff.py /usr/local/bin/clang-format-diff
     ```

## Installation

```bash
cd $THIS_REPO
git submodule update --init
echo "source $(realpath ./dev_tools/linter/setup_linter.sh)" >> ~/.bashrc  # Or the matching file for
                                                   # your shell.
bash
```

## Usage

Just commit. The linter will stop any commit that looks suspicious and will let you know what is wrong.
You can either fix the issues manually, or try a more brute force approach by running `clang-format`.
