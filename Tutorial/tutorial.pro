TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../Code/inc

include(1.platform/platform.pri)
include(2.context/context.pri)
include(3.queue/queue.pri)
include(4.program/program.pri)
include(5.buffer/buffer.pri)
include(6.kernel/kernel.pri)
include(7.events/events.pri)
include(8.matrix/matrix.pri)
include(9.minimum/minimum.pri)
include(10.image/image.pri)
include(11.test/test.pri)
