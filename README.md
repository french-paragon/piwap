# Piwap
Piwap stand for Pictures warping App. It is a small project I started when I found out that Phatch was removed from the official ubuntu repositories since it was no longer maintained.

I looked first if it was worth forking phatch, but I don't have so much expercience develloping in Python, and my people strongly advised on different forum to continue with this codebase.

So, i started from stratch in c++, with QT as a toolkit for the graphical user interface and OpenCV for the processing backend. I choose OpenCV as I knew it, and I knew its array class is pretty flexible to interact with other c++ libs. But let's face it, OpenCV is not a tool for artists, so instead I switch to ImageMagick as a backend, which has its own drawbacks but is better suited for artistic purposes.

Piwap is published under the GPL License. For the moment it is still under heavy devellopement and I will only provide appimages for Linux (recent linux distribution, as I build with a recent version of glibc. In the future I might setup a better build environnement to target older linux systems and windows.

Currently Piwap support only two kind of resize operations and the saving operations. I'm looking for inputs. Feel free to use github issue tracker to propose operations you'd like to see implemented.
