                  Moulin Bouge v0.5
                  =================

0. Table of contents
====================

    0. Table of contents (you're here)
    1. The goal(s) of moulin bouge
    2. How do I compile and link moulin bouge to my project?
        a. What are all those libraries?
        b. Use moulin bouge as a dynamic library
        c. Use moulin bouge as a static library
        d. Embed moulin bouge right into your project
        e. An important note to MSVC users
        f. Integration using CMake
    3. Learning to use moulin bouge
        a. Game developers
            The examples
        b. Game artists
            The examples
    4. Known issues

1. The goal(s) of moulin bouge
==============================

    Moulin bouge wants to be a highly __flexible__ __skeletal__ animation library
    targeting __modern__ graphics hardware.

    This implies that there is no plan for adding morph targets or other animation
    types that do not rely on bones and weights.

    Moulin bouge is flexible in several ways:
        - materials are just "dictionaries" of key-values,
        - everything has a "name", not an ID,
        - high decoupling of mesh, skeleton, animation, materials and skins,
        - meshes have generic vertex attributes.

    Moulin bouge has been created because the game Arkana-FTS was in the need
    of a better, more flexible and modern library than Cal3D, which it was using.

2. How do I compile and link moulin bouge to my project?
========================================================

    Moulin bouge uses CMake as a build system. You have three choices to include
    moulin bouge in your project:

    a. What are all those libraries?
    --------------------------------
        While developing moulin bouge, I wanted to keep things separate, that's
        why now there are several libraries in bouge. To sum it up:

            The core, which you definitely need, is made of:
                - "bouge"
                - "bouge-math" contains matrix, vector, quaternion and time
                               function calculus. You can also use that one
                               without even using bouge, if you like it.

            Then, there are the various file formats moulin bouge understands:
                - "bouge-xmlio" This is the native moulin bouge XML file format.
                                It includes all files ending in ".bx*".
                - "bouge-cal3dxio" This can load the XML files from the Cal3D
                                   library: xrf, xsf, xmf, xaf

            Finally, there are the XML helper libraries, which you only need if
            you are using one of the XML kind of file formats mentioned above:
                - "bouge-xml-common" this is the core, you definitely need that.

            Besides the XML core, you can choose which XML parser you want to use.
            You'll then need the parser library itself and an adapter for moulin
            bouge. Moulin bouge comes bundled with the TinyXML parser:
                - "bouge-tinyxml" includes both the TinyXML parser and adapter.

            Thus, in a typical use case, you'll need all of the following libs:
                - bouge
                - bouge-math
                - bouge-xmlio
                - bouge-xml-common
                - bouge-tinyxml

    b. Use moulin bouge as a dynamic library
    ----------------------------------------
        In this case, you have to set some CMake settings. You can do this by
        running the CMake-GUI on windows or by either running "ccmake" on unix/linux
        or editing the CMakeCache.txt file by hand.

        The setting you need to change is BUILD_SHARED_LIBS, which you should
        set to TRUE.

        Then, the compilation will give you dynamic libraries (.dll, .so) which
        you can link to your project.

        Note that you need not to define any preprocessors in your project.

    c. Use moulin bouge as a static library
    ---------------------------------------
        In this case, you can just execute CMake and the default settings will
        create all moulin bouge libraries as static libraries (.lib, .a).

        Then, you can link the libraries you need (see a.) to your project.

        Finally, define the "BOUGE_STATIC" preprocessor in your project.

    d. Embed moulin bouge right into your project
    ---------------------------------------------
        If you go with this approach, you'll need to place all the files from the
        ''src'' and ''include'' folders into the folders of your project and
        compile them along with your project.

        You'll need to add the preprocessor definition "BOUGE_STATIC" to your compiler.

        You may omit the files for the formats you don't want in the ''IOModules''
        subfolder, but keep in mind that all XML file formats need the files in
        ''XMLParserCommon''.

        That's it, you're ready to use moulin bouge and need no link step.

    e. An important note to MSVC users
    ----------------------------------
        In MSVC, it is an important question whether to link to the CRT (C runtime)
        statically or dynamically and whether to use the multithreaded version or not.

        Moulin bouge always uses the multithreaded library, but you have the
        choice whether you want to use the static or dynamic library.

        You need to set the BUILD_STATICCRT CMake option to true to use the static
        runtime library.

        Note that this version not always works yet. If you have too much trouble,
        you might just go with "d. Embed moulin bouge right into your project".

    f. Integration using CMake
    --------------------------
        In the case your project also uses CMake, you can try to find if bouge
        is installed on the pc already by using the Findbouge CMake macro.

        TODO: Examples, more explanation?

3. Learning to use moulin bouge
===============================

    There are mainly two groups of users that may want (or need) to use moulin bouge:

    a. Game developers
    ------------------
        Game developers have to learn how to program using the moulin bouge
        library, how to load, draw and manipulate moulin bouge models, or even
        how to add an importer for another model format to moulin bouge.

        Game developers should read the "moulin bouge tutorial" available either
        in the "doc" subdirectory or on moulin bouge's webpage.

        The examples:
        - - - - - - -
            Game developers usually learn by examples. That's why moulin bouge
            comes with a bunch of examples on its usage.

            To compile the examples, you have to turn that option ON in CMake,
            as it is off by default! (Because of compilation times and deps.)

            The examples show various use-cases, you should mainly inspire from
            the viewer-glut and staticviewer-glut examples:

                - moulin-bouge-glut: (TODO)
                    A full demo program with a little GUI useful for testing
                    any aspect of the model, using OpenGL

                - viewer-glut:
                    An very complete and optimized example on how to use animated
                    models using modern OpenGL and GLSL.

                - staticviewer-glut:
                    A viewer optimized for static models, that is, without
                    any animations. Uses modern OpenGL and GLSL.

                - skeletonviewer-glut:
                    Shows only the (animated) skeleton, not the mesh.
                    suboptimally programmed.

                - cal3dx-to-bougexml:
                    A converter loading files in Cal3D xml format and saving
                    them again in bouge's native XML format.

                - io:
                    Loading and saving again.

                - plot-tf:
                    Create a plot (png) of any time function

    b. Game artists
    ---------------
        Artists most often need to know what kind of things the moulin bouge
        format supports and what they have to care about when exporting their
        models.

        There is one generic document which explains some concepts of bouge
        (like skins) to the artist, it's called "moulin bouge concepts (for artists)".

        Then, there is one document for each modelling program/exporter, which
        explains the specific things that the artist using that program needs
        to care about. These documents are called "moulin bouge exporter from XXX".

        All these documents can be found on the moulin bouge's webpage and in
        the "doc" subdirectory here.

            The examples:
            - - - - - - -

            You can also have a look at the ''examples/data'' subfolder, as
            it includes the "source" files of most example models. If you
            are lucky, the source file has been made using your program :)

4. Known Issues
===============

    All known issues are written down (somehow) in the TODO.txt file. If you
    encounter an issue that is not mentioned there, please see the website on
    how to report a bug.
