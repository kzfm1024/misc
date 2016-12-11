#!/bin/bash

gdbus-codegen --interface-prefix org.gtk.GDBus.Example.ObjectManager. \
              --generate-c-code generated-code                        \
              --c-namespace Example                                   \
              --c-generate-object-manager                             \
              --generate-docbook generated-docs                       \
              gdbus-example-objectmanager.xml
