=======
Nouveau
=======
This document provides a general overview of the driver, contribution details,
internal API documentation, and glossary. It is strictly for the nouveau kernel
driver. The userspace side of nouveau is documented at the `nouveau freedesktop
wiki <https://nouveau.freedesktop.org/wiki/>`_.

If you'd like to improve this documentation, great! Refer to the :doc:`Sphinx
introduction </doc-guide/sphinx>` and :doc:`/doc-guide/kernel-doc` documents
for details on how to do so.

Module Parameters
=================
A number of module parameters are supported. Each parameter requires a value.
These can be passed via the kernel command line using the format
"nouveau.<parameter-name>=<value>". Boolean values should use 1 for true and 0
for false.

.. kernel-doc:: drivers/gpu/drm/nouveau/nouveau_drm.c


Driver Overview
===============
The driver is located in ``drivers/gpu/drm/nouveau/``.

.. note:: The latest pending patches for nouveau are available as an
   `out-of-tree driver <https://github.com/skeggsb/nouveau>`_.

Within the driver, there are several distinct sections. The reason for this is
that NVIDIA hardware is partitioned into "privileged" and "user" blocks.

The general architecture is described in the diagram below.

.. kernel-render:: DOT
   :alt: Nouveau Architecture Diagram
   :caption: Nouveau Architecture Diagram

   digraph "Nouveau" {
      node [shape=box]
      "Userspace" -> "DRM APIs"
      "Userspace" -> "NVIF APIs"
      "DRM APIs" -> "NVIF APIs"
      "NVIF APIs" -> "NVKM APIs"
      "NVKM APIs" -> "Hardware-specific Interfaces"
   }


NVKM
----
The NVidia Kernel Module (NVKM) is responsible for the low-level resource
management of the privileged portions of the hardware. Almost all direct
register access is performed in this layer. The functionality of the underlying
hardware is exposed by NVKM as objects of a particular class, and in general
these are identified with the same class IDs that NVIDIA uses.

Some classes, such as :term:`channels`, have a block of registers associated with
them that are intended to be directly accessed by an unprivileged client. NVKM
allows objects to be "mapped" by a client to support this.

The NVKM layer sits closest to the hardware and services requests issued by
clients.


NVIF
----
Atop the NVKM sits the NVidia Interface (NVIF) library, which defines a client
interface that can be used to interact with the NVKM server. NVIF is intended
to be usable both in the kernel and in userspace. This is accomplished with
drivers that implement the interface defined in struct nvif_driver. Clients
within the kernel use with an NVIF client backed with
struct nvif_driver_nvkm.

This design allows userspace direct access to the registers of :term:`channels`
it allocates and allows it to submit work to the GPU without going through the
kernel.


DRM
---
The DRM layer of nouveau uses the NVIF to implement the interfaces of a DRM
driver, such as modesetting, command submission to :term:`channels`
from userspace, synchronization between userspace clients, and so on.

.. note:: All interaction with the NVKM layer inside the kernel should happen
   through NVIF.  Historically this has not been the case, so there may still
   be legacy code that bypasses NVIF and directly calls NVKM.

Nouveau's DRM driver is defined in the aptly-named nouveau_drm.c file. The
files in the driver directory's root provide all the functionality required for
the DRM driver. Kernel mode-setting is implemented in the dispnv* directories
and is abstracted in the ``nouveau_display.h`` interface.

For details on what is required to implement these interfaces interfaces, refer
to the :doc:`drm-internals`, :doc:`drm-kms`, and :doc:`drm-uapi` documents.


API
===
This section includes the kernel-docs for nouveau APIs.


NVKM
----
Privileged, low-level resource management interfaces.

engine/disp.h
~~~~~~~~~~~~~
.. kernel-doc:: drivers/gpu/drm/nouveau/include/nvkm/engine/disp.h


Glossary
========
There are a number of NVIDIA-specific terms in the code as well as the
documentation.

.. glossary::

   EVO
   NVD
       In pre-Volta architectures, the Evolution (EVO) controller is used to
       interact with display memory-mapped IO registers via a pushbuffer.  In
       Volta architectures and newer, the NVDisplay controller takes the place
       of the EVO controller, although it has slightly different semantics.

   channels
        Channels are hardware blocks that consumes methods from a
        direct-memory-accessed command stream.

