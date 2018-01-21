/*
 * Nom:			GOODMAN
 * PRENOM:		David
 * # ETUDIANT:	21401471
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <uvsqgraphics.h>
#include <constants.h>
#include "read_write.h"

int loadlvl(int BOARD[][NUMB_BOX_HEIGHT])
{
    FILE* file=NULL;
    int i=0, j=0;

    file=fopen("sasquatch1", "r");
    if (file == NULL)
        return 0;

    fgets(...)

    fclose(file);
    return 1;
}
