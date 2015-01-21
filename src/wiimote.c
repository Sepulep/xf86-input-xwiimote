/*
 * XWiimote
 *
 * Copyright (c) 2011-2013 David Herrmann <dh.herrmann@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <xorg-server.h>
#include <inttypes.h>
#include <stdlib.h>
#include <math.h>
#include <xf86.h>

#include "wiimote.h"


BOOL wiimote_ir_is_active(struct wiimote *wiimote,
                          struct wiimote_config *config,
                          struct xwii_event *ev)
{
  return ir_is_active(&wiimote->ir, &config->ir, ev);
}

static void configure_wiimote_keys(struct wiimote_config *config,
                                   char const * prefix,
                                   InputInfoPtr info) {
  char option_key[100];

  snprintf(option_key, sizeof(option_key), "%sLeft", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_LEFT], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sRight", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_RIGHT], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sUp", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_UP], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sDown", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_DOWN], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sA", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_A], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sB", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_B], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sPlus", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_PLUS], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sMinus", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_MINUS], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sHome", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_HOME], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sOne", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_ONE], option_key, info);

  snprintf(option_key, sizeof(option_key), "%sTwo", prefix);
	configure_key(&config->keys[WIIMOTE_KEY_TWO], option_key, info);
}

void configure_wiimote(struct wiimote_config *config,
                       char const * prefix,
                       struct wiimote_config *defaults,
                       InputInfoPtr info) {
	const char *motion;

  memset(config, 0, sizeof(struct wiimote_config));

  if (defaults) {
	  memcpy(config, defaults, sizeof(struct wiimote_config));
  }

	motion = xf86FindOptionValue(info->options, "MotionSource");
	if (!motion)
		motion = "";

/* TODO
	if (!strcasecmp(motion, "accelerometer")) {
		dev->motion = MOTION_ABS;
		dev->motion_source = SOURCE_ACCEL;
		dev->ifs |= XWII_IFACE_ACCEL;
	} else if (!strcasecmp(motion, "ir")) {
		dev->motion = MOTION_ABS;
		dev->motion_source = SOURCE_IR;
		dev->ifs |= XWII_IFACE_IR;
	} else if (!strcasecmp(motion, "motionplus")) {
		dev->motion = MOTION_REL;
		dev->motion_source = SOURCE_MOTIONPLUS;
		dev->ifs |= XWII_IFACE_MOTION_PLUS;
	}
*/

  configure_ir(&config->ir, prefix, info);
  configure_accelerometer(&config->accelerometer, prefix, info);
  configure_motionplus(&config->motionplus, "MP", info);

  configure_wiimote_keys(config, prefix, info);
}

void handle_wiimote_key(struct wiimote *wiimote,
                        struct wiimote_config *config,
                        struct xwii_event *ev,
                        unsigned int state,
                        InputInfoPtr info)
{
  unsigned int keycode;
  struct key *key;
  struct key_config *key_config;

  keycode = ev->v.key.code;
  key = &wiimote->keys[keycode];
  key_config = &config->keys[keycode];

  handle_key(key, key_config, state, info);
}

void handle_wiimote_ir(struct wiimote *wiimote,
                       struct wiimote_config *config,
                       struct xwii_event *ev,
                       unsigned int state,
                       InputInfoPtr info)
{
  handle_ir(&wiimote->ir, &config->ir, ev, info);
}

void handle_wiimote_motionplus(struct wiimote *wiimote,
                               struct wiimote_config *config,
                               struct xwii_event *ev,
                               unsigned int state,
                               InputInfoPtr info)
{
  handle_motionplus(&wiimote->motionplus, &config->motionplus, ev, info);
}

void handle_wiimote_accelerometer(struct wiimote *wiimote,
                                  struct wiimote_config *config,
                                  struct xwii_event *ev,
                                  unsigned int state,
                                  InputInfoPtr info)
{
  handle_accelerometer(&wiimote->accelerometer, &config->accelerometer, ev, info);
}


/* TODO
void refresh_wiimote(struct xwiimote_dev *dev)
{
	int ret;

	ret = xwii_iface_open(dev->iface, dev->ifs);
	if (ret)
		xf86IDrvMsg(dev->info, X_INFO, "Cannot open all requested interfaces\n");
}
*/


void preinit_wiimote(struct wiimote_config *config) {
  preinit_motionplus(&config->motionplus);
  preinit_ir(&config->ir);
}

