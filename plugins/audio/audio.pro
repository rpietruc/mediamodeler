CONFIG  += ordered
TEMPLATE = subdirs
SUBDIRS += \
    audioframes \
    acoustictubetransform \
    spectrumtransform \
    formanttransform \
    spectrumplotdestination \
    spectrogramdestination \
    sounddevicesource \
    sounddevicedestination \
    soundplotdestination \
    soundfiltertransform \
    soundlinearpredictiontransform
#SUBDIRS += soundlinearpredictionfiledestination
#SUBDIRS += soundextractchanneltransform
#SUBDIRS += soundlinespectralpairstransform
