/* SPDX-License-Identifier: MIT */

/**
 * DOC: Overview
 *
 * Interfaces for working with the display engine.
 */

#ifndef __NVKM_DISP_H__
#define __NVKM_DISP_H__

/**
 * nvkm_disp() - Get a &struct nvkm_disp from a &struct nvkm_engine.
 *
 * @p: A &struct nvkm_engine reference.
 *
 * Return: The &struct nvkm_disp that contains the given engine.
 */
#define nvkm_disp(p) container_of((p), struct nvkm_disp, engine)
#include <core/engine.h>
#include <core/event.h>

/**
 * struct nvkm_disp - Represents a display engine.
 *
 * This structure is for <some abstraction here>. It has <some assumptions
 * about its usage here>.
 */
struct nvkm_disp {
    /** @func: Chipset-specific vtable for manipulating the display. */
	const struct nvkm_disp_func *func;

    /** @engine: The engine for this display. */
	struct nvkm_engine engine;

    /** @head: list of heads attached to this display. */
	struct list_head head;

    /** @ior: List of IO resources for this display. */
	struct list_head ior;

    /** @outp: List of outputs for this display. */
	struct list_head outp;

    /** @conn: List of connectors for this display. */
	struct list_head conn;

    /** @hpd: An event that fires when something happens I guess. */
	struct nvkm_event hpd;

    /** @vblank: An event that fires and has some relation to the vblank. */
	struct nvkm_event vblank;

    /** @client: The oproxy (?) client for this display. */
	struct nvkm_oproxy *client;
};

int nv04_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int nv50_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int g84_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gt200_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int g94_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int mcp77_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gt215_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int mcp89_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gf119_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gk104_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gk110_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gm107_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gm200_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gp100_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gp102_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int gv100_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
int tu102_disp_new(struct nvkm_device *, int, struct nvkm_disp **);
#endif
