Bouge v0.5
==========


0. Table of contents
====================

1. The goal(s) of bouge
2. How do I compile and link bouge to my project?
    1. What are all those libraries?
    2. Use bouge as a dynamic library
    3. Use bouge as a static library
    4. Embed bouge right into your project
    5. An important note to MSVC users
    6. Integration using CMake
3. Learning to use bouge
    1. Game developers
    2. Game artists
4. Known issues


1. The goal(s) of bouge
=======================

Bouge wants to be a highly __flexible__ __skeletal__ animation library
targeting __modern__ graphics hardware.

This implies that there is currently no plan for adding morph targets or other
animation types that do not rely on bones and weights.

Bouge is flexible in several ways:

    - materials are just "dictionaries" of key-values,
    - everything has a "name", not an ID,
    - high decoupling of mesh, skeleton, animation, materials and skins,
    - meshes have generic vertex attributes.

Bouge has been created because the game [Arkana-FTS](http://arkana-fts.org) was in the need
of a better, more flexible and modern library than Cal3D, which it was using.


2. How do I compile and link bouge to my project?
=================================================

Bouge uses CMake as a build system. You have three choices to include bouge in your project:

2.1. What are all those libraries?
----------------------------------

While developing bouge, I wanted to keep things separate, that's
why now there are several libraries in bouge. To sum it up:

The core, which you definitely need, is made of:
- `bouge`
- `bouge-math` contains matrix, vector, quaternion and time function calculus.
    You can also use that one without even using bouge, if you like it.

Then, there are the various file formats bouge understands:
- `bouge-xmlio` is the native bouge XML file format; it includes all files ending in `.bx*`.
- `bouge-cal3dxio` This can load the XML files from the Cal3D library: `xrf`, `xsf`, `xmf`, `xaf`

Finally, there are the XML helper libraries, which you only need if
you are using one of the XML kind of file formats mentioned above:
- `bouge-xml-common` this is the core, you definitely need that.

Besides the XML core, you can choose which XML parser you want to use.
You'll then need the parser library itself and an adapter for bouge.
Bouge comes bundled with the TinyXML parser:
- `bouge-tinyxml` includes both the TinyXML parser and adapter.

Thus, in a standard use case, you'll need all of the following libs:
- `bouge`
- `bouge-math`
- `bouge-xmlio`
- `bouge-xml-common`
- `bouge-tinyxml`

2.2. Use bouge as a dynamic library
-----------------------------------

In this case, you have to set some CMake settings. You can do this by
running the CMake-GUI on windows or by either running "ccmake" on unix/linux
or editing the CMakeCache.txt file by hand.

The setting you need to change is `BUILD_SHARED_LIBS`, which you should set to `TRUE`.

Then, the compilation will give you dynamic libraries (.dll, .so) which you can link to your project.

Note that you need not to define any preprocessors in your project.

2.3. Use bouge as a static library
----------------------------------

In this case, you can just execute CMake and the default settings will
create all bouge libraries as static libraries (.lib, .a).

Then, you can link the libraries you need (see a.) to your project.

Finally, define the `BOUGE_STATIC` preprocessor in your project.

2.4. Embed bouge right into your project
----------------------------------------

If you go with this approach, you'll need to place all the files from the
`src` and `include` folders into the folders of your project and
compile them along with your project.

You'll need to add the preprocessor definition `BOUGE_STATIC` to your compiler.

You may omit the files for the formats you don't want in the `IOModules` subfolder,
but keep in mind that all XML file formats need the files in `XMLParserCommon`.

That's it, you're ready to use bouge and need no link step.

2.5. An important note to MSVC users
------------------------------------

In MSVC, it is an important question whether to link to the CRT (C runtime)
statically or dynamically and whether to use the multithreaded version or not.

Bouge always uses the multithreaded library, but you have the
choice whether you want to use the static or dynamic library.

You need to set the `BUILD_STATICCRT` CMake option to `TRUE` to use the static
runtime library.

Note that this version not always works yet. If you have too much trouble,
you might just go with option "d. Embed bouge right into your project".

2.6. Integration using CMake
----------------------------

In the case your project also uses CMake, you can try to find if bouge
is installed on the pc already by using the Findbouge CMake macro.

    TODO: Examples, more explanation?

3. Learning to use bouge
========================

There are mainly two groups of users that may want (or need) to use bouge:

3.1. Game developers
--------------------

Game developers have to learn how to program using the bouge library,
how to load, draw and manipulate bouge models, and even how to add an importer
for another model format to bouge.

Game developers should read the "bouge tutorial" available either
in the `doc` subdirectory or on bouge's webpage.

### The examples:

Game developers usually learn by examples. That's why bouge comes with a bunch of examples on its usage.

To compile the examples, you have to turn that option `ON` in CMake,
as it is off by default! (Because of compilation times and deps.)

The examples show various use-cases, you should mainly inspire from
the `viewer-glut` and `staticviewer-glut` examples:

- `viewer-glut`: A very complete and optimized example on how to render animated models in OpenGL 3/4.x and GLSL.
- `staticviewer-glut`: A viewer optimized for static models, that is, without any animations. Also in modern OpenGL.
- `skeletonviewer-glut`: Shows only the (animated) skeleton, without the mesh. Suboptimally programmed.
- `cal3dx-to-bougexml`: A converter loading files in Cal3D xml format and saving them again in bouge's native XML format.
- `io`: Loading and saving again.
- `plot-tf`: Create a plot (png) of any time function

3.2. Game artists
-----------------

Artists most often need to know what kind of things the bouge format supports
and what they have to care about when exporting their models.

There is one generic document which explains some concepts of bouge
(like skins) to the artist, it's called "bouge concepts (for artists)".

Then, there is one document for each modelling program/exporter, which
explains the specific things that the artist using that program needs
to care about. These documents are called "bouge exporter from XXX".

All these documents can be found on bouge's webpage and in the `doc` subdirectory here.

### The examples:

You can also have a look at the `examples/data` subfolder, as
it includes the "source" files of most example models. If you
are lucky, the source file has been made using your program :)


4. Known Issues
===============

All known issues are written down (somehow) in the TODO.txt file. If you
encounter an issue that is not mentioned there, please see the website on
how to report a bug.
