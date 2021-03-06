/* 
 * razer_chroma_drivers - a driver/tools collection for razer chroma devices
 * (c) 2015 by Tim Theede aka Pez2001 <pez2001@voyagerproject.de> / vp
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */
#ifndef _RAZER_DAEMON_EFFECTS_H_
#define _RAZER_DAEMON_EFFECTS_H_

#define RAZER_EFFECT_NO_INPUT_USED 0
#define RAZER_EFFECT_FIRST_INPUT_USED 1
#define RAZER_EFFECT_SECOND_INPUT_USED 2

//#define RAZER_EFFECT_DEVICE_ 0


typedef int (*razer_effect_open)(struct razer_fx_render_node *render);//called when effect is added to a render node
typedef int (*razer_effect_close)(struct razer_fx_render_node *render);//called when effect is removed from a render node
typedef int (*razer_effect_reset)(struct razer_fx_render_node *render);//called every time a effect is called for the first time in a render node(loops trigger too) - a daemon manual reset command can be used too
typedef int (*razer_effect_parameter_changed)(struct razer_fx_render_node *render,struct razer_parameter *parameter);//called every time a parameter of a render_node is changed/added
typedef int (*razer_effect_added_to_render_nodes)(struct razer_fx_render_node *render);//called every time a render_node is added to the daemons render_nodes
typedef int (*razer_effect_update)(struct razer_fx_render_node *render);//called every frame - returns 0 if execution is completed
typedef int (*razer_effect_handle_event)(struct razer_fx_render_node *render,struct razer_chroma_event *event);//handler for key events
//typedef int (*razer_effect_mouse_event)(struct razer_fx_render_node *render,int rel_x,int rel_y,int buttons_mask);//handler for mouse events
typedef int (*razer_effect_dbus_event)(struct razer_fx_render_node *render);//handler for dbus events

typedef void (*razer_effect_init)(struct razer_daemon *daemon);//called when libray is loaded
typedef void (*razer_effect_shutdown)(struct razer_daemon *daemon);//called when libray is unloaded


struct razer_effect
{
	int id;
	char *name;
	char *description;
	int effect_class;//1=render effect / 2=supplemental effect (non rendering)
	int fps;
	int parameters_uid;
	list *parameters;//struct razer_parameter
	int input_usage_mask;//bitmask showing the effects usage of inputs
	razer_effect_open open;
	razer_effect_reset reset;
	razer_effect_added_to_render_nodes added_to_render_nodes;
	razer_effect_parameter_changed parameter_changed;
	razer_effect_close close;
	razer_effect_update update;
	razer_effect_handle_event handle_event;
	razer_effect_dbus_event dbus_event;
	void *tag;
};

char *daemon_effect_to_json(struct razer_effect *effect, int final);

struct razer_effect *daemon_create_effect(void);
void daemon_free_effect(struct razer_effect *effect);
int daemon_register_effect(struct razer_daemon *daemon,struct razer_effect *effect);
int daemon_unregister_effect(struct razer_daemon *daemon,struct razer_effect *effect);
struct razer_effect *daemon_get_effect(struct razer_daemon *daemon,int uid);
struct razer_effect *daemon_create_effect_instance(struct razer_daemon *daemon,struct razer_effect *lib_effect);

struct razer_parameter *daemon_effect_get_parameter_by_index(struct razer_effect *effect,int index);
int daemon_effect_add_parameter(struct razer_effect *effect,struct razer_parameter *parameter);

#endif
