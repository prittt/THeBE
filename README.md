# THeBE: The THinning evaluation BEnchmark &nbsp;&nbsp;&nbsp; [![Build Status](https://travis-ci.org/prittt/THeBE.svg?branch=master)](https://travis-ci.org/prittt/THeBE)

<p align="justify">Please include the following reference when citing the THeBE project/dataset:</p>

- <p align="justify"> Bolelli, Federico; Grana, Costantino "Improving the Performance of Thinning Algorithms with Directed Rooted Acyclic Graphs" 20th International Conference on Image Analysis and Processing, 2019. <a title="BibTex" href="http://imagelab.ing.unimore.it/files2/thebe/THEBE_ICIAP2019_BibTex.html">BibTex</a>. <!--<a title="Download" href="https://iris.unimore.it/retrieve/handle/11380/1155728/186233/2018_JRTIP_Towards_Reliable_Experiments_on_the_Performance_of_Connected_Components_Labeling_Algorithms.pdf"><img src="https://raw.githubusercontent.com/prittt/YACCLAB/master/doc/pdf_logo.png" alt="Download." /></a>--></p>

<p align="justify">
THeBE is an open source <i>C++</i> project that enables researchers to test thinning algorithms under extremely variable points of view, running and testing algorithms on a collection of datasets <!--described below-->. The benchmark provides <i>correctness</i> and average run-time (<i>average</i>) tests.
</p>

## Requirements

<p align="justify">To correctly install and run THeBE following packages, libraries and utilities are needed:</p>

- CMake 3.0.0 or higher (https://cmake.org),
- OpenCV 3.0 or higher (http://opencv.org),
- Gnuplot (http://www.gnuplot.info/),
- One of your favourite IDE/compiler: Visual Studio 2013 or higher, Xcode 5.0.1, gcc 4.7 or higher, .. (with C++11 support)

Notes for gnuplot:
- on Windows system: be sure of adding gnuplot to the system path to allow the automatic charts generation.
- on MacOS system: 'pdf terminal' seems to be not available, 'postscript' is used instead.

<a name="inst"></a>
## Installation (refer to the image below)

- <p align="justify">Clone the GitHub repository (HTTPS clone URL: https://github.com/prittt/THeBE.git) or simply download the full master branch zip file and extract it (e.g THeBE folder).</p>
- <p align="justify">Install software in THeBE/bin subfolder (suggested) or wherever you want using CMake (point 2 of the example image). Note that CMake should automatically find the OpenCV path whether correctly installed on your OS (3), download the THeBE Dataset (be sure to check the box if you want to download it (4) or to select the correct path if the dataset is already on your file system (5)), and create a C++ project for the selected IDE/compiler (7-8).</p>

![Cmake](doc/readme_github.png)

- <p align="justify">Set the <a href="#conf">configuration file (config.yaml)</a> placed in the installation folder (bin in this example) in order to select desired tests.</p>

- <p align="justify">Open the project, compile and run it: the work is done!</p>

<!--
## How to include a YACCLAB algorithm into your own project?

<p align="justify">If your project requires a Connected Components Labeling algorithm and you are not interested in the whole YACCLAB benchmark you can use the <i>connectedComponent</i> function of the OpenCV library which implements the BBDT and SAUF algorithms since version 3.2.</p>
<p align="justify">Anyway, when the <i>connectedComponents</i> function is called, lot of additional code will be executed together with the core function. If your project requires the best performance you can include an algorithm implemented in YACCLAB adding the following files to your project:</p>
<ol>
  <li><i>labeling_algorithms.h</i> and <i>labeling_algorithms.cc</i> which define the base class from which every algorithm derives from.</li>
  <li><i>label_solver.h</i> and <i>label_solver.cc</i> which cointain the implementation of labels solving algorithms.</li>
  <li><i>memory_tester.h</i> and <i>performance_evaluator.h</i> just to make things work without changing the code.</li>
  <li><i>headers</i> and <i>sources</i> files of the required algorithm/s. The association between algorithms and headers/sources files is reported in the table below.</li>
</ol>  
 <table>
  <tr>
    <th>Algorithm Name</th>
    <th width="130">Authors</th>
    <th>Year</th>
    <th>Acronym</th>
    <th>Required Files</th>
    <th>Templated on Labels Solver</th>
  </tr>
  <tr>
    <td align="center">-</td>
    <td align="center">L. Di Stefano,<br>A. Bulgarelli <sup><a href="#DiStefano">[3]</a></sup></td>
    <td align="center">1999</td>
    <td align="center">DiStefano</td>
    <td align="center"><i>labeling_distefano_1999.h</i></td>
    <td align="center">NO</td>
  </tr>
  <tr>
    <td align="center">Contour Tracing</td>
    <td align="center">F. Chang,</br>C.J. Chen,</br>C.J. Lu <sup><a href="#CT">[1]</a></sup></td>
    <td align="center">1999</td>
    <td align="center">CT</td>
    <td align="center"><i>labeling_fchang_2003.h</i></td>
    <td align="center">NO</td>
  </tr>
  <tr>
    <td align="center">Configuration Transition Based</td>
    <td align="center">L. He,</br>X. Zhao,</br>Y. Chao,</br>K. Suzuki <sup><a href="#CTB">[7]</a></sup></td>
    <td align="center">1999</td>
    <td align="center">CTB</td>
    <td align="center"><i>labeling_he_2014.h</i>, <i>labeling_he_2014_graph.inc</i>
    <td align="center">YES</td>
  </tr>
  <tr>
    <td align="center">Scan Array-based with Union Find</td>
    <td align="center">K. Wu,</br>E. Otoo,</br>K. Suzuki <sup><a href="#SAUF">[6]</a></sup></td>
    <td align="center">2009</td>
    <td align="center">SAUF</td>
    <td align="center"><i>labeling_wu_2009.h</i>, <i>labeling_wu_2009_tree.inc</i></td>
    <td align="center">YES</td>
  </tr>
    <tr>
    <td align="center">Stripe-Based Labeling Algorithm</td>
    <td align="center">H.L. Zhao,</br>Y.B. Fan,</br>T.X. Zhang,</br>H.S. Sang <sup><a href="#SBLA">[8]</a></sup></td>
    <td align="center">2010</td>
    <td align="center">SBLA</td>
    <td align="center"><i>labeling_zhao_2010.h</i></td>
    <td align="center">NO</td>
  </tr>
  <tr>
    <td align="center">Block-Based with Decision Tree</td>
    <td align="center">C. Grana,</br>D. Borghesani,</br>R. Cucchiara <sup><a href="#BBDT">[4]</a></sup></td>
    <td align="center">2010</td>
    <td align="center">BBDT</td>
    <td align="center"><i>labeling_grana_2010.h</i>, <i>labeling_grana_2010_tree.inc</i></td>
    <td align="center">YES</td>
  </tr>
  <tr>
    <td align="center">Block-Based with Binary Decision Trees</td>
    <td align="center">W.Y. Chang,</br>C.C. Chiu,</br>J.H. Yang <sup><a href="#CCIT">[2]</a></sup></td>
    <td align="center">2015</td>
    <td align="center">CCIT</td>
    <td align="center"><i>labeling_wychang_2015.h</i>, <i>labeling_wychang_2015_tree.inc</i>, <i>labeling_wychang_2015_tree_0.inc</i></td>
    <td align="center">YES</td>
  </tr>
  <tr>
    <td align="center">Light Speed Labeling</td>
    <td align="center">L. Cabaret,</br>L. Lacassagne,</br>D. Etiemble <sup><a href="#LSL_STD">[5]</a></sup></td>
    <td align="center">2016</td>
    <td align="center">LSL_STD<small><sup>I</sup></small></br>LSL_STDZ<small><sup>II</sup></small></br>LSL_RLE<small><sup>III</sup></small></td>
    <td align="center"><i>labeling_lacassagne_2016.h</i>, <i>labeling_lacassagne_2016_code.inc</i></td>
    <td align="center">YES<small><sup>IV</sup></small></td>
  </tr>
  <tr>
    <td align="center">Pixel Prediction</td>
    <td align="center">C.Grana,</br>L. Baraldi,</br>F. Bolelli <sup><a href="#PRED">[9]</a></sup></td>
    <td align="center">2016</td>
    <td align="center">PRED</td>
    <td align="center"><i>labeling_grana_2016.h</i>, <i>labeling_grana_2016_forest.inc</i>, <i>labeling_grana_2016_forest_0.inc</i>
    <td align="center">YES</td>
  </tr>
  <tr>
    <td align="center">Directed Rooted Acyclic Graph</td>
    <td align="center">F. Bolelli,</br>L. Baraldi,</br>C. Grana <sup><a href="#DRAG">[19]</a></sup></td>
    <td align="center">2018</td>
    <td align="center">DRAG</td>
    <td align="center"><i>labeling_bolelli_2018.h</i>, <i>labeling_grana_2018_drag.inc</i></td>
    <td align="center">YES</td>
  </tr>
  <tr>
    <td align="center">Null Labeling</td>
    <td align="center">F. Bolelli,</br>M. Cancilla,</br>L. Baraldi,</br>C. Grana <sup><a href="#YACCLAB_JRTIP">[18]</a></sup></td>
    <td align="center">2018</td>
    <td align="center">NULL<small><sup>V</sup></small></td>
    <td align="center"><i>labeling_null.h</i></td>
    <td align="center">NO</td>
  </tr>
</table>

(<small>I</small>) standard version </br>
(<small>II</small>) with zero-offset optimization </br>
(<small>III</small>) with RLE compression </br>
(<small>IV</small>) only on TTA and UF </br>
(<small>V</small>) it only copies the pixels from the input image to the output one simply defining a lower bound limit for the execution time of CCL algorithms on a given machine and dataset.

### Example of Algorithm Usage Outside the Benchmark

```c++
#include "labels_solver.h"
#include "labeling_algorithms.h"
#include "labeling_grana_2010.h" // To include the algorithm code (BBDT in this example)

#include <opencv2/opencv.hpp>

using namespace cv;

int main()
{
    BBDT<UFPC> BBDT_UFPC; // To create an object of the desired algorithm (BBDT in this example)
                          // templated on the labels solving strategy. See the README for the
                          // complete list of the available labels solvers, available algorithms
                          // (N.B. non all the algorithms are templated on the solver) and their
                          // acronyms.

    BBDT_UFPC.img_ = imread("test_image.png", IMREAD_GRAYSCALE); // To load into the CCL object
                                                                 // the BINARY image to be labeled

    threshold(BBDT_UFPC.img_, BBDT_UFPC.img_, 100, 1, THRESH_BINARY); // Just to be sure that the
                                                                      // loaded image is binary

    BBDT_UFPC.PerformLabeling(); // To perform Connected Components Labeling!

    Mat1i output = BBDT_UFPC.img_labels_; // To get the output labeled image  
    unsigned n_labels = BBDT_UFPC.n_labels_; // To get the number of labels found in the input img

    return EXIT_SUCCESS;
}
```

<a name="conf"></a>
## Configuration File
<p align="justify">A <tt>YAML</tt> configuration file placed in the installation folder lets you to specify which kind of tests should be performed, on which datasets and on which algorithms. A complete description of all configuration parameters is reported below.</p>

- <i>perform</i> - dictionary which specifies the <a href="#conf">kind of tests</a> to perform:
```yaml
perform:
  correctness:        false
  average:            true
  average_with_steps: false
  density:            false
  granularity:        false
  memory:             false
```

- <i>correctness_tests</i> - dictionary indicating the kind of correctness tests to perform:
```yaml
correctness_tests:
  eight_connectivity_standard: true
  eight_connectivity_steps:    true
  eight_connectivity_memory:   true
```

- <i>tests_number</i> - dictionary which sets the number of runs for each test available:
```yaml
tests_number:
  average:            10
  average_with_steps: 10
  density:            10
  granularity:        10
```

- <i>algorithms</i> - list of algorithms on which apply the chosen tests:
```yaml
algorithms:
  - SAUF_RemSP
  - SAUF_TTA
  - BBDT_RemSP
  - BBDT_UFPC
  - CT
  - labeling_NULL
```

- <i>check_datasets</i>, <i>average_datasets</i>, <i>average_ws_datasets</i> and <i>memory_datasets</i> - lists of <a href="#conf">datasets</a> on which, respectively, correctness, average, average_ws and memory tests should be run:
```yaml
...
average_datasets: ["3dpes", "fingerprints", "hamlet", "medical", "mirflickr", "tobacco800", "xdocs"]
...
```

- <i>paths</i> - dictionary with both input (datasets) and output (results) paths. It is automatically filled by Cmake during the creation of the project:
```yaml
paths: {input: "<datasets_path>", output: "<output_results_path>"}
```

- <i>write_n_labels</i> - whether to report the number of connected components in the output files:
```yaml
write_n_labels: false
```

- <i>color_labels</i> - whether to output a colored version of labeled images during tests:
```yaml
color_labels: {average: false, density: false}
```

- <i>save_middle_tests</i> - dictionary specifying, separately for every test, whether to save the output of single runs, or only a summary of the whole test:
```yaml
save_middle_tests: {average: false, average_with_steps: false, density: false, granularity: false}
```

## How to Extend YACCLAB with New Algorithms

Work in progress.

<a name="datasets"></a>
## The YACCLAB Dataset
<p align="justify">The YACCLAB dataset includes both synthetic and real images and it is suitable for a wide range of applications, ranging from document processing to surveillance, and features a significant variability in terms of resolution, image density, variance of density, and number of components. All images are provided in 1 bit per pixel PNG format, with 0 (black) being background and 1 (white) being foreground. The dataset will be automatically downloaded by CMake during the installation process as described in the <a href="#inst">installation</a> paragraph, or it can be found at http://imagelab.ing.unimore.it/yacclab. Images are organized by folders as follows: </p>

- <b>Synthetic Images</b>:
	- <b>Classical:<sup><a href="#BBDT">4</a></sup></b><p align="justify"> A set of synthetic random noise images who contain black and white random noise with 9 different foreground densities (10% up to 90%), from a low resolution of 32x32 pixels to a maximum resolution of 4096x4096 pixels, allowing to test the scalability and the effectiveness of different approaches when the number of labels gets high. For every combination of size and density, 10 images are provided for a total of 720 images. The resulting subset allows to evaluate performance both in terms of scalability on the number of pixels and on the number of labels (density). </p>
	- <b>Granularity:<sup><a href="#LSL">4</a></sup></b><p align="justify"> This dataset allows to test algorithms varying not only the pixels density but also their granularity <i>g</i> (<i>i.e.</i>, dimension of minimum foreground block), underlying the behaviour of different proposals when the number of provisional labels changes. All the images have a resolution of 2048x2048 and are generated with the Mersenne Twister MT19937 random number generator implemented in the <i>C++</i> standard and starting with a "seed" equal to zero. Density of the images ranges from 0% to 100% with step of 1% and for every density value 16 images with pixels blocks of <i>gxg</i> with <i>g</i> ∈ [1,16] are generated. Moreover, the procedure has been repeated 10 times for every couple of density-granularity for a total of 16160 images.</p>

- <b>MIRflickr:<sup><a href="#MIRFLICKR">10</a></sup></b><p align="justify"> Otsu-binarized version of the MIRflickr dataset, publicly available under a Creative Commons License. It contains 25,000 standard resolution images taken from Flickr. These images have an average resolution of 0.17 megapixels, there are few connected components (495 on average) and are generally composed of not too complex patterns, so the labeling is quite easy and fast.</p>

- <b>Hamlet:</b><p align="justify"> A set of 104 images scanned from a version of the Hamlet found on Project Gutenberg (http://www.gutenberg.org). Images have an average amount of 2.71 million of pixels to analyze and 1447 components to label, with an average foreground density of 0.0789. </p>

- <b>Tobacco800:<sup><a href="#TOBACCO1">11</a>,<a href="#TOBACCO2">12</a>,<a href="#TOBACCO3">13</a></sup></b><p align="justify"> A set of 1290 document images. It is a realistic database for document image analysis research as these documents were collected and scanned using a wide variety of equipment over time. Resolutions of documents in Tobacco800 vary significantly from 150 to 300 DPI and the dimensions of images range from 1200 by 1600 to 2500 by 3200 pixels. Since CCL is one of the initial preprocessing steps in most layout analysis or OCR algorithms, hamlet and tobacco800 allow to test the algorithm performance in such scenarios. </p>

- <b>3DPeS:<sup><a href="#3DPES">14</a></sup></b> <p align="justify"> It comes from 3DPeS (3D People Surveillance Dataset), a surveillance dataset designed mainly for people re-identification in multi camera systems with non-overlapped fields of view. 3DPeS can be also exploited to test many other tasks, such as people detection, tracking, action analysis and trajectory analysis. The background models for all cameras are provided, so a very basic technique of motion segmentation has been applied to generate the foreground binary masks, i.e.,  background subtraction and fixed thresholding. The analysis of the foreground masks to remove small connected components and for nearest neighbor matching is a common application for CCL. </p>

- <b>Medical:<sup><a href="#MEDICAL">15</a></sup></b><p align="justify"> This dataset is composed by histological images and allow us to cover this fundamental medical field. The process used for nuclei segmentation and binarization is described in  <a href="#MEDICAL">[12]</a>. The resulting dataset is a collection of 343 binary histological images with an average amount of 1.21 million of pixels to analyze and 484 components to label. </p>

- <b>Fingerprints:<sup><a href="#FINGERPRINTS">16</a></sup></b><p align="justify"> This dataset counts 960 fingerprint images collected by using low-cost optical sensors or synthetically generated. These images were taken from the three Verification Competitions FCV2000, FCV2002 and FCV2004. In order to fit CCL application, fingerprints have been binarized using an adaptive threshold and then negated in order to have foreground pixel with value 255. Most of the original images have a resolution of 500 DPI and their dimensions range from 240 by 320 up to 640 by 480 pixels. </p>

<a name="tests"></a>
## Available Tests

- <b>Average run-time tests:</b> <p align="justify"> execute an algorithm on every image of a dataset. The process can be repeated more times in a single test, to get the minimum execution time for each image: this allows to get more reproducible results and overlook delays produced by other running processes. It is also possible to compare the execution speed of different algorithms on the same dataset: in this case, selected algorithms (see <a href="#conf">Configuration File</a> for more details) are executed sequentially on every image of the dataset. Results are presented in three different formats: a plain text file, histogram charts (.pdf/.ps), either in color or in gray-scale, and a LaTeX table, which can be directly included in research papers.</p>

- <b>Density and size tests:</b> <p align="justify"> check the performance of different CCL algorithms when they are executed on images with varying foreground density and size. To this aim, a list of algorithms selected by the user is run sequentially on every image of the test_random dataset. As for run-time tests, it is possible to repeat this test for more than one run. The output is presented as both plain text and charts(.pdf/.ps). For a density test, the mean execution time of each algorithm is reported for densities ranging from 10% up to 90%, while for a size test the same is reported for resolutions ranging from 32x32 up to 4096x4096.</p>

- <b>Memory tests:</b> <p align="justify"> are useful to understand the reason for the good performances of an algorithm or in general to explain its behavior. Memory tests compute the average number of accesses to the label image (i.e the image used to store the provisional and then the final labels for the connected components), the average number of accesses to the binary image to be labeled, and, finally, the average number of accesses to data structures used to solve the equivalences between label classes. Moreover, if an algorithm requires extra data, memory tests summarize them as ``other'' accesses and return the average. Furthermore, all average contributions of an algorithm and dataset are summed together in order to show the total amount of memory accesses. Since counting the number of memory accesses imposes additional computations, functions implementing memory access tests are different from those implementing run-time and density tests, to keep run-time tests as objective as possible.</p>

## Examples of YACCLAB Output Results
Work in progress.
## References

<p align="justify"><em><a name="CT">[1]</a> F. Chang, C.-J. Chen, and C.-J. Lu, “A linear-time component-labeling algorithm using contour tracing technique,” Computer Vision and Image Understanding, vol. 93, no. 2, pp. 206–220, 2004.</em></p>
<p align="justify"><em><a name="CCIT">[2]</a> W.-Y.  Chang,  C.-C.  Chiu,  and  J.-H.  Yang,  “Block-based  connected-component  labeling  algorithm  using  binary  decision  trees,” Sensors, vol. 15, no. 9, pp. 23 763–23 787, 2015.</em></p>
<p align="justify"><em><a name="DiStefano">[3]</a> L.  Di  Stefano  and  A.  Bulgarelli,  “A  Simple  and  Efficient  Connected Components Labeling Algorithm,” in International Conference on Image Analysis and Processing. IEEE, 1999, pp. 322–327.</em></p>
<p align="justify"><em><a name="BBDT">[4]</a> C.  Grana,  D.  Borghesani,  and  R.  Cucchiara,  “Optimized  Block-based Connected Components Labeling with Decision Trees,” IEEE Transac-tions on Image Processing, vol. 19, no. 6, pp. 1596–1609, 2010.</em></p>
<p align="justify"><em><a name="LSL_STD">[5]</a> L. Lacassagne and B. Zavidovique, “Light speed labeling: efficient connected component labeling on risc architectures,” Journal of Real-Time Image Processing, vol. 6, no. 2, pp. 117–135, 2011</em>.</p>
<p align="justify"><em><a name="SAUF">[6]</a> K. Wu, E. Otoo, and K. Suzuki, Optimizing two-pass connected-component labeling algorithms,” Pattern Analysis and Applications, vol. 12, no. 2, pp. 117–135, 2009.</em></p>
<p align="justify"><em><a name="CTB">[7]</a> L.  He,  X.  Zhao,  Y.  Chao,  and  K.  Suzuki, Configuration-Transition-
Based  Connected-Component  Labeling, IEEE  Transactions  on  Image Processing, vol. 23, no. 2, pp. 943–951, 2014.</em></p>
<p align="justify"><em><a name="SBLA">[8]</a> H.  Zhao,  Y.  Fan,  T.  Zhang,  and  H.  Sang, Stripe-based  connected components  labelling, Electronics  letters,  vol.  46,  no.  21,  pp.  1434–1436, 2010.</em></p>
<p align="justify"><em><a name="PRED">[9]</a> C. Grana, L. Baraldi, and F. Bolelli, Optimized Connected Components Labeling  with  Pixel  Prediction, in Advanced  Concepts  for  Intelligent Vision Systems, 2016.</em></p>
<p align="justify"><em><a name="MIRFLICKR">[10]</a> M. J. Huiskes and M. S. Lew, “The MIR Flickr Retrieval Evaluation,” in MIR ’08: Proceedings of the 2008 ACM International Conference on Multimedia Information Retrieval. New York, NY, USA: ACM, 2008. [Online]. Available: http://press.liacs.nl/mirflickr/</em></p>
<p align="justify"><em><a name="TOBACCO1">[11]</a> G. Agam, S. Argamon, O. Frieder, D. Grossman, and D. Lewis, “The Complex Document Image Processing (CDIP) Test Collection Project,” Illinois Institute of Technology, 2006. [Online]. Available: http://ir.iit.edu/projects/CDIP.html</em></p>
<p align="justify"><em><a name="TOBACCO2">[12]</a> D. Lewis, G. Agam, S. Argamon, O. Frieder, D. Grossman, and J. Heard, “Building a test collection for complex document information processing,” in Proceedings of the 29th annual international ACM SIGIR conference on Research and development in information retrieval. ACM, 2006, pp. 665–666.</em></p>
<p align="justify"><em><a name="TOBACCO3">[13]</a> “The Legacy Tobacco Document Library (LTDL),” University of California, San Francisco, 2007. [Online]. Available: http://legacy. library.ucsf.edu/</em></p>
<p align="justify"><em><a name="3DPES">[14]</a> D. Baltieri, R. Vezzani, and R. Cucchiara, “3DPeS: 3D People Dataset for Surveillance and Forensics,” in Proceedings of the 2011 joint ACM workshop on Human gesture and behavior understanding. ACM, 2011, pp. 59–64.</em></p>
<p align="justify"><em><a name="MEDICAL">[15]</a> F. Dong, H. Irshad, E.-Y. Oh, M. F. Lerwill, E. F. Brachtel, N. C. Jones, N. W. Knoblauch, L. Montaser-Kouhsari, N. B. Johnson, L. K. Rao et al., “Computational Pathology to Discriminate Benign from Malignant Intraductal Proliferations of the Breast,” PloS one, vol. 9, no. 12, p. e114885, 2014.</em></p>
<p align="justify"><em><a name="FINGERPRINTS">[16]</a> D. Maltoni, D. Maio, A. Jain, and S. Prabhakar, Handbook of fingerprint
recognition. Springer Science & Business Media, 2009.</em></p>
<p align="justify"><em><a name="YACCLAB">[17]</a> C.Grana, F.Bolelli, L.Baraldi, and R.Vezzani, YACCLAB - Yet Another Connected Components Labeling Benchmark, Proceedings of the 23rd International Conference on Pattern Recognition, Cancun, Mexico, 4-8 Dec 2016.</em></p>
<p align="justify"><em><a name="YACCLAB_JRTIP">[18]</a> Bolelli, Federico; Cancilla, Michele; Baraldi, Lorenzo; Grana, Costantino "Towards Reliable Experiments on the Performance of Connected Components Labeling Algorithms" Journal of Real-Time Image Processing, 2018.</em></p>
<p align="justify"><em><a name="DRAG">[19]</a> Bolelli, Federico; Baraldi, Lorenzo; Cancilla, Michele; Grana, Costantino "Connected Components Labeling on DRAGs" Proceedings of the 23rd International Conference on Pattern Recognition, Beijing, China, 20-24 Aug 2018.</em></p>
-->
