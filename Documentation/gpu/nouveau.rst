=======
Nouveau
=======
Nouveau is the free/libre driver for NVIDIA GPUs. This documentation is for the
kernel mode-setting driver, and is meant to complement the general `nouveau`_
documentation, which contains end user documentation and a general overview of
the project, and `envytools`_, which covers reverse-engineering and the
hardware architecture of the individual card families.

Issues with this driver are tracked on Nouveau's freedesktop.org `issue
tracker`_.

If you'd like to improve this documentation, great! Refer to the :doc:`Sphinx
introduction </doc-guide/sphinx>` and :doc:`/doc-guide/kernel-doc` documents
for details on how to do so.

User Guide
==========
As noted in the introduction, this document is for the kernel portion of the
nouveau driver and is primarily focused on documentation for developers. This

TODO Layout

Introduction: Cover envytools, nouveau wiki documentation and role, and how
this is for kernel specific bits.

User Documentation: Point to the nouveau wiki, down to module parameters.

Kernel-specific troubleshooting, Module Parameters

Developer Guide
  - Driver Overview (maybe point to envytools here)
  - External APIs (include/uapi/drm/nouveau_drm.h, any sysfs stuff)?
  - Internal APIs
  - Debugging and Development Tools
  - Contribution Guide


Module Parameters
=================
A number of module parameters are provided to tweak the behavior of the driver.
These are provided to ease debugging issues and users that need to set a
parameter to fix an issue they are experiencing should report this as a bug on
the `issue tracker`_.

Each parameter requires a value. These can be passed via the kernel command
line using the format "nouveau.<parameter-name>=<value>". Boolean values should
use 1 for true and 0 for false.

.. kernel-doc:: drivers/gpu/drm/nouveau/nouveau_drm.c


Driver Overview
===============
The driver is located in ``drivers/gpu/drm/nouveau/``.

.. note:: The latest pending patches for nouveau are available as an
   `out-of-tree driver <https://github.com/skeggsb/nouveau>`_.

Within the driver, there are several distinct sections. The reason for this is
that NVIDIA hardware is partitioned into "privileged" and "user" blocks. For
more details on the particulars of NVIDIA's hardware, consult `envytools`_.

The general module architecture from userspace to the hardware lay is described
in the diagram below.

.. kernel-render:: DOT
   :alt: Nouveau Software Architecture Diagram
   :caption: Nouveau Software Architecture Diagram

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
In this section the interfaces provided by nouveau, both internal and public
userspace interfaces, are described.

Userspace Interfaces
--------------------
Like other DRM drivers, much of the interface exposed to userspace is
documented in :doc:`drm-uapi`. There are a few nouveau-specific interfaces.

debugfs
~~~~~~~
Nouveau exposes the following :doc:`DebugFS </filesystems/debugfs>` files.

.. warning:: Debugfs is intended for developers and test tooling only.

nv_crc/flip_threshold
^^^^^^^^^^^^^^^^^^^^^
.. kernel-doc:: drivers/gpu/drm/nouveau/dispnv50/crc.c
   :doc: nv_crc/flip_threshold


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


.. _nouveau: https://nouveau.freedesktop.org/
.. _envytools: https://envytools.readthedocs.io/
.. _issue track: https://gitlab.freedesktop.org/drm/nouveau/-/issues
