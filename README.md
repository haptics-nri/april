AprilTag-MEX
============

This is the AprilTag-C library from the [University of Michigan](https://april.eecs.umich.edu/software/apriltag.html), along with a MEX file allowing its use from within Matlab.

Usage
=====

First, compile AprilTag-C:

```sh
$ make
```

It has no dependencies that you won't find already on a POSIX system. In my experience, it does not work on Windows. I have no interest at all in wasting time making it do so, but PRs are welcome.

From within Matlab, compile the MEX:

```matlab
>> mex apriltag_mex.c -I. -Icommon -L. -lapriltag
```

Find the tags in an image:

```matlab
>> img = load('bluefox1.png') % an RGB image
>> gray = rgb2gray(img);
>> [id, center, p1, p2, p3, p4] = apriltag_mex(gray);
```

Find the tags in all PNGs in the current directory:

```matlab
>> [names, ids, centers, p1s, p2s, p3s, p4s] = process_all;
```

License
=======

The AprilTag-C library is BSD licensed by the University of Michigan. I modified it by adding the `image_u8_create_from_gray` function.
The accompanying MEX file and script are BSD licensed by me.

