/*
	Solution for the problem 368 of aceptaelreto.com
	- Cociendo huevos - 
    Copyright (C) 2018 hnko
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int num_eggs, size, time, rest;
	while(1){
		scanf("%d %d", &num_eggs, &size); if (num_eggs == 0 && size == 0) return 0;

		rest = num_eggs % size;
		if(rest != 0) time = ((num_eggs/size) +1)*10;
		else time = (num_eggs/size)*10;

		printf("%d\n", time);
	}
	return 0;
}