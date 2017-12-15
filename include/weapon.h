/*
 * Netcrack -- a multiplayer roguelike
 * Copyright (C) 2008  June R. Tate-Gans, Jonathan L. Tate
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
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef WEAPON_H
#define WEAPON_H

#include <dice.h>
#include <effect.h>
#include <ammo.h>

/**
 * The kind of weapon a weapon_type is.
 */
typedef enum {
    MELEE,
    ENERGY,
    PROJECTILE,
    THROWN
} weapon_kind;

/**
 * A structure to define a generic weapon.
 */
typedef struct {
    weapon_kind type;
    char *name;
    float weight;
    int num_hands;
    int range;

    die damage;
    char *damage_type;

    effect_type *effects;
    int *effect_chances;

    ammo_type *ammo;
    int ammo_usage;
} weapon_type;

#endif /* WEAPON_H */
