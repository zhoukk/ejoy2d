#include "screenshot.h"
#include "sprite.h"
#include "shader.h"
#include "texture.h"
#include "opengl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char*
_get_screenshot_pixels(int x, int y, int w, int h) {
	if (w <= 0 || h <= 0) {
		return NULL;
	}

	glFinish();

	size_t size = w * h * 4;
	GLubyte* pixels = (GLubyte*) malloc(size);
	if (!pixels) {
		return NULL;
	}
	glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	return pixels;
}

static void
_fill_sprite_with_texure(int tex_id, struct sprite* s, int w, int h) {
	s->parent = NULL;
	s->type = TYPE_PICTURE;
	s->id = 0;
	s->t.mat = NULL;
	s->t.color = 0xffffffff;
	s->t.additive = 0;
	s->t.program = PROGRAM_DEFAULT;

	s->start_frame = 0;
	s->total_frame = 0;
	s->frame = 0;
	s->visible = true;
	s->message = false;
	s->multimount = false;
	s->name = NULL;
	s->material = NULL;
	memset(&s->data, 0, sizeof(s->data));

	struct pack_picture * pp = (struct pack_picture *) malloc(sizeof(*pp));
	pp->n = 1;
	struct pack_quad * q = &pp->rect[0];
	q->texid = tex_id;
	int w2 = w * 4;
	int h2 = h * 4;

	q->texture_coord[0] = 0;
	q->texture_coord[1] = 0;
	q->texture_coord[2] = 0xffff;
	q->texture_coord[3] = 0;
	q->texture_coord[4] = 0xffff;
	q->texture_coord[5] = 0xffff;
	q->texture_coord[6] = 0;
	q->texture_coord[7] = 0xffff;

	q->screen_coord[0] = -w2;
	q->screen_coord[1] = h2;
	q->screen_coord[2] = w2;
	q->screen_coord[3] = h2;
	q->screen_coord[4] = w2;
	q->screen_coord[5] = -h2;
	q->screen_coord[6] = -w2;
	q->screen_coord[7] = -h2;
	s->s.pic = pp;
}

int
screenshot(int x, int y, int w, int h, int tex_id, struct sprite* spr, unsigned char** ppixles) {
	if (!ppixles || !spr)
		return -1;

	unsigned char* pixels = _get_screenshot_pixels(x, y, w, h);
	texture_load(tex_id, TEXTURE_RGBA8, w, h, pixels, 0);
	_fill_sprite_with_texure(tex_id, spr, w, h);
	*ppixles = pixels;
	return 0;
}

void
release_screenshot(int tex_id, unsigned char* pixels) {
	if (tex_id) {
		texture_unload(tex_id);
	}

	if (pixels) {
		free(pixels);
	}
}

