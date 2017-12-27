project('teensy-core', 'c', 'cpp',
    version : '1.0',
    default_options : [
      'b_staticpic=false',
      ])

assert(meson.is_cross_build(), 'Teensy projects can only be built in a cross build environment.')

teensy_core_cflags = [
    #too many warnings due to non virtual dtor
    '-Wno-non-virtual-dtor',
    ]

teensy_core_ldflags = [
    ]

teensy_core_incdir = include_directories([
  'teensy3',
  'teensy3/avr',
  'teensy3/util',
  ])

teensy_core_src = [
    'teensy3/analog.c',
    'teensy3/AudioStream.cpp',
    'teensy3/avr_emulation.cpp',
    'teensy3/DMAChannel.cpp',
    'teensy3/eeprom.c',
    'teensy3/HardwareSerial1.cpp',
    'teensy3/HardwareSerial2.cpp',
    'teensy3/HardwareSerial3.cpp',
    'teensy3/HardwareSerial4.cpp',
    'teensy3/HardwareSerial5.cpp',
    'teensy3/HardwareSerial6.cpp',
    'teensy3/IntervalTimer.cpp',
    'teensy3/IPAddress.cpp',
    'teensy3/keylayouts.c',
    'teensy3/main.cpp',
    'teensy3/math_helper.c',
    'teensy3/memcpy-armv7m.S',
    'teensy3/memset.S',
    'teensy3/mk20dx128.c',
    'teensy3/new.cpp',
    'teensy3/nonstd.c',
    'teensy3/pins_teensy.c',
    'teensy3/Print.cpp',
    'teensy3/serial1.c',
    'teensy3/serial2.c',
    'teensy3/serial3.c',
    'teensy3/serial4.c',
    'teensy3/serial5.c',
    'teensy3/serial6.c',
    'teensy3/serial6_lpuart.c',
    'teensy3/ser_print.c',
    'teensy3/Stream.cpp',
    'teensy3/Tone.cpp',
    'teensy3/touch.c',
    'teensy3/usb_audio.cpp',
    'teensy3/usb_desc.c',
    'teensy3/usb_dev.c',
    'teensy3/usb_flightsim.cpp',
    'teensy3/usb_inst.cpp',
    'teensy3/usb_joystick.c',
    'teensy3/usb_keyboard.c',
    'teensy3/usb_mem.c',
    'teensy3/usb_midi.c',
    'teensy3/usb_mouse.c',
    'teensy3/usb_mtp.c',
    'teensy3/usb_rawhid.c',
    'teensy3/usb_seremu.c',
    'teensy3/usb_serial.c',
    'teensy3/usb_touch.c',
    'teensy3/WMath.cpp',
    'teensy3/WString.cpp',
    'teensy3/yield.cpp',
    ]

teensy_core = static_library('teensy-core', teensy_core_src,
    cpp_args : teensy_core_cflags,
    include_directories : teensy_core_incdir)

teensy_core_dep = declare_dependency(
    link_with : teensy_core,
    include_directories : teensy_core_incdir,
    )
