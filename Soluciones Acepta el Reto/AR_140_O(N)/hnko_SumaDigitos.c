/*
	Solution for the problem 140 of aceptaelreto.com
	- Suma de digitos - 
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
#include <string.h>

int main(int argc, char const *argv[])
{
	char c[13];
	for(; ;){
		scanf("%s", c); if(c[0] == '-') return 0; /* TLE with strcmp() */
		int len = strlen(c), sum=0, i;
		for(i=0; i<len; i++){
			sum += c[i]-48;
			if(i != len-1 ) printf("%c + ", c[i]);
			else printf("%c = %d\n", c[i], sum);
		}	
	}
	return 0;
}