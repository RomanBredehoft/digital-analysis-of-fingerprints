# Digital analysis of fingerprints

Here is our project, made by four MSIAM students (M1 level), as well as the associated report. The used programming language is C++11.

## Usage

```bash
cd build/
make all
./demo/file.cpp ../data/image.png
```

This project contains 8 demonstration files to observe the different results:

\begin{itemize}
  \item[\textbf{anisotropic_demo: }] computes the weak pressure simulation of fingerprints
  \item[\textbf{convolution_demo: }] computes different convolution on the images
  \item[\textbf{isotropic_demo: }] computes teh weak pressure simulation an isotropical way
  \item[\textbf{morphological_demo: }] computes dilations, erosions, closing and opening on images, to simulate dry and mosit fingerprints\item[\textbf{opti_demo: }] computes the translation and rotations on an image to find the ones which minimizes the difference between the image and an other ones
  \item[\textbf{rectangle_demo: }] computes rectangles on the image
  \item[\textbf{rotation_demo: }] computes rotations and warps on the image
  \item[\textbf{sym_demo: }] computes different symmetries on the image
\end{itemize}
