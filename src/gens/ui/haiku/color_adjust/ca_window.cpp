/***************************************************************************
 * Gens: (Haiku) Color Adjustment Window.                                  *
 *                                                                         *
 * Copyright (c) 1999-2002 by Stéphane Dallongeville                       *
 * Copyright (c) 2003-2004 by Stéphane Akhoun                              *
 * Copyright (c) 2008-2009 by David Korth                                  *
 * Copyright (c) 2009 by Phil Costin                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU General Public License as published by the   *
 * Free Software Foundation; either version 2 of the License, or (at your  *
 * option) any later version.                                              *
 *                                                                         *
 * This program is distributed in the hope that it will be useful, but     *
 * WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.           *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ca_window.h"
#include "gens/gens_window.h"

// Unused Parameter macro.
#include "libgsft/gsft_unused.h"

// Gens includes.
#include "emulator/md_palette.hpp"
#include "emulator/g_main.hpp"
#include "emulator/g_update.hpp"
#include "gens_core/vdp/vdp_io.h"
#include "util/file/rom.hpp"

#ifdef GENS_DEBUGGER
#include "debugger/debugger.hpp"
#endif /* GENS_DEBUGGER */


/**
 * ca_window_show(): Show the Color Adjustment window.
 */
void ca_window_show(void)
{
}


/**
 * ca_window_close(): Close the Color Adjustment window.
 */
void ca_window_close(void)
{
}

