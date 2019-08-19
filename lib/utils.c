/**
 *  Copyright (C) 2011-2012  Juho Vähä-Herttua
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// {0x01,0x33,..} -> 0133...
int
utils_hwaddr_raop(char *str, int strlen, const char *hwaddr, int hwaddrlen)
{
	int i,j;

	/* Check that our string is long enough */
	if (strlen == 0 || strlen < 2*hwaddrlen+1)
		return -1;

	/* Convert hardware address to hex string */
	for (i=0,j=0; i<hwaddrlen; i++) {
		int hi = (hwaddr[i]>>4) & 0x0f;
		int lo = hwaddr[i] & 0x0f;

		if (hi < 10) str[j++] = '0' + hi;
		else         str[j++] = 'A' + hi-10;
		if (lo < 10) str[j++] = '0' + lo;
		else         str[j++] = 'A' + lo-10;
	}

	/* Add string terminator */
	str[j++] = '\0';
	return j;
}
// {0x01,0x33,..} -> 01:33...
int
utils_hwaddr_airplay(char *str, int strlen, const char *hwaddr, int hwaddrlen)
{
	int i,j;

	/* Check that our string is long enough */
	if (strlen == 0 || strlen < 2*hwaddrlen+hwaddrlen)
		return -1;

	/* Convert hardware address to hex string */
	for (i=0,j=0; i<hwaddrlen; i++) {
		int hi = (hwaddr[i]>>4) & 0x0f;
		int lo = hwaddr[i] & 0x0f;

		if (hi < 10) str[j++] = '0' + hi;
		else         str[j++] = 'a' + hi-10;
		if (lo < 10) str[j++] = '0' + lo;
		else         str[j++] = 'a' + lo-10;

		str[j++] = ':';
	}

	/* Add string terminator */
	if (j != 0) j--;
	str[j++] = '\0';
	return j;
}

char*
utils_hexstr_to_byte(char *str, int strlen, int* data_len)
{
	int byte_len = strlen / 2;
	*data_len = byte_len;
	char* data = malloc(byte_len);
	for (int i = 0; i < byte_len; i++) {
		sscanf(str, "%2hhx", &data[i]);
		str += 2;
	}
	return data;
}