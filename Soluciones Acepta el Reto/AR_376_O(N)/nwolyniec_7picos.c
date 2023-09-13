
/*
	Solution for the problem 376 of aceptaelreto.com
	- Siete picos - 
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
	int num_mountains, mountains[1000], i, j, cases, count;
	
	while(1){
		scanf("%d", &num_mountains);
		if (num_mountains == 0) return 0;
		/* save the mountais */
		for(i=0; i<num_mountains; i++){
			scanf("%d", &mountains[i]);
		}
		/* find how many peaks are in the roller coaster */
		for(count=0, cases=0, i=1, j=num_mountains-1; cases < num_mountains ; cases++){
			if((mountains[j] < mountains[cases]) &&  (mountains[cases] > mountains[i])){
				count++;
			}
			/* to make circular i and j */
			i = (i+1) % num_mountains;
			j = (j+1) % num_mountains;
		}
		printf("%d\n", count);
	}
	return 0;

}