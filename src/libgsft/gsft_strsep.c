/*******************************************************************************
 * libgsft: Common functions.                                                  *
 * gsft_strsep.c: strsep() implementation for older systems.                   *
 *                                                                             *
 * MDP implementation Copyright (c) 2008-2009 by David Korth                   *
 *                                                                             *
 * Original implementation derived from the GNU C Library.                     *
 * Copyright (C) 1992, 93, 96, 97, 98, 99, 2004 Free Software Foundation, Inc. *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the       *
 * Free Software Foundation; either version 2 of the License, or (at your      *
 * option) any later version.                                                  *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but         *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                  *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.               *
 *******************************************************************************/

#include "gsft_strsep.h"

// C includes.
#include <string.h>

char *strsep(char **stringp, const char *delim)
{
	char *begin, *end;
	
	begin = *stringp;
	if (begin == NULL)
		return NULL;
	
	/* A frequent case is when the delimiter string contains only one
	   character.  Here we don't need to call the expensive `strpbrk'
	   function and instead work using `strchr'.  */
	if (delim[0] == '\0' || delim[1] == '\0')
	{
		char ch = delim[0];
		
		if (ch == '\0')
			end = NULL;
		else
		{
			if (*begin == ch)
				end = begin;
			else if (*begin == '\0')
				end = NULL;
			else
				end = strchr(begin + 1, ch);
		}
	}
	else
	{
		/* Find the end of the token.  */
		end = strpbrk(begin, delim);
	}
	
	if (end)
	{
		/* Terminate the token and set *STRINGP past NUL character.  */
		*end++ = '\0';
		*stringp = end;
	}
	else
	{
		/* No more delimiters; this is the last token.  */
		*stringp = NULL;
	}
	
	return begin;
}
