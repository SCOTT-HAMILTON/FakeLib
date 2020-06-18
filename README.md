# Fake Lib

![powered by Pulseaudio](https://www.freedesktop.org/software/pulseaudio/logo.png)

![Powered by Pulseaudio](icons/powered_by_pulseaudio.png)


![Build Passing](https://travis-ci.org/SCOTT-HAMILTON/FakeLib.svg?branch=master)

Fake is a pulseaudio wrapper library written in c++.

# Meson Building 
  To build it, run : 
  ```sh
  meson setup build
  ninja -C build
  ninja -C install
  ```

# Usage
 FakeLib can run multiple pulseaudio commands at once. The structure used runs the commands in the same server connection. 
 The basic usage consists of four steps : 
  1. Stacking commands that you want to run.
  2. Running them.
  4. Extracting the resuts you're interesed in.
  5. Using the extracted results.

In code, this would translate to
```cpp
// The FakeLib instance to work with
FakeLib fakeLib;

// stacking and running commands (steps 1 and 2) is done here
auto result = fakeLib
	     .get_module_list()
	     .get_sink_list()
	     .get_source_list()
	     .get_source_output_list()
	     .run_commands();

// We extract the results
auto module_list = extract<info_list<module_infos_t>>(result);
auto sink_list = extract<info_list<sink_infos_t>>(result);
auto source_list = extract<info_list<source_infos_t>>(result);
auto source_output_list = extract<info_list<source_output_infos_t>>(result);

// And we print them with those helper functions
print_module_list(module_list);
print_sink_list(sink_list);
print_source_list(source_list);
print_source_output_list(source_output_list);
```

This code outputs this on my laptop : 

```
=======[ Module info #1 ]=======
Name: module-device-restore
Index: 0

=======[ Module info #2 ]=======
Name: module-stream-restore
Index: 1

[...]

=======[ Module info #15 ]=======
Name: module-rescue-streams
Index: 14

=======[ Module info #16 ]=======
Name: module-always-sink
Index: 15

=======[ Sink info #1 ]=======
Name: alsa_output.pci-0000_00_1f.3.analog-stereo
Index: 0
Description: Audio interne Stéréo analogique

=======[ Source info #1 ]=======
Name: alsa_output.pci-0000_00_1f.3.analog-stereo.monitor
Index: 0
Description: Monitor of Audio interne Stéréo analogique

=======[ Source info #2 ]=======
Name: alsa_input.pci-0000_00_1f.3.analog-stereo
Index: 1
Description: Audio interne Stéréo analogique

=======[ Source Output info #1 ]=======
Name: recStream
Source: 1
Index: 0
Source Process Binary Name: .Discord-wrapped
```

## Integration

FakeLib provides a pkg-config file.

### Requirements

Fake requires libpulse

### Help

FakeLib is just a little project, but feel free to fork, change, extend or correct the code.


License
----
FakeLib is delivered as it is under the well known MIT License


**References that helped**
 - [pulseaudio IRC] : <#pulseaudio>
 - [pulseaudio under the hood] : <https://gavv.github.io/articles/pulseaudio-under-the-hood/>
 - [pulseaudio documentation] : <https://freedesktop.org/software/pulseaudio/doxygen/>
 - [pulseaudio introspect API interface] :
   <https://freedesktop.org/software/pulseaudio/doxygen/introspect_8h.html>

[//]: # (These are reference links used in the body of this note and get stripped out when the markdown processor does its job. There is no need to format nicely because it shouldn't be seen. Thanks SO - http://stackoverflow.com/questions/4823468/store-comments-in-markdown-syntax)



   [pulseaudio IRC]: <#pulseaudio>
   [pulseaudio under the hood]: <https://gavv.github.io/articles/pulseaudio-under-the-hood/>
   [pulseaudio documentation]: <https://freedesktop.org/software/pulseaudio/doxygen/>
   [pulseaudio introspect API interface]:
   <https://freedesktop.org/software/pulseaudio/doxygen/introspect_8h.html>
