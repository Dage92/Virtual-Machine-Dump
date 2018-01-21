/****************************************************/
/*   Projet      : Balade dans le metro             */
/*   Date        : mai 2009                         */
/*   Auteur      : Climoo                           */
/*   Fichier     : bm_reseau.h                      */
/*   Description : En-tete contenant les            */
/*   les codes des caracteres speciaux (accents...) */
/*   Contient aussi des macros permettant de tester */
/*   si un caractere est un caractere special       */
/*                                                  */
/* **************************************************/

#ifndef _BM_CARAC_H

#define _BM_CARAC_H

#define E_AIGU_MIN L'\x00E9'
#define E_GRAV_MIN L'\x00E8'
#define E_TREM_MIN L'\x00EB'
#define E_CIRC_MIN L'\x00EA'

#define A_AIGU_MIN L'\x00E1'
#define A_GRAV_MIN L'\x00E0'
#define A_TREM_MIN L'\x00E4'
#define A_CIRC_MIN L'\x00E2'

#define I_AIGU_MIN L'\x00ED'
#define I_GRAV_MIN L'\x00EC'
#define I_TREM_MIN L'\x00EF'
#define I_CIRC_MIN L'\x00EE'

#define O_AIGU_MIN L'\x00F3'
#define O_GRAV_MIN L'\x00F2'
#define O_TREM_MIN L'\x00F6'
#define O_CIRC_MIN L'\x00F4'

#define U_AIGU_MIN L'\x00FA'
#define U_GRAV_MIN L'\x00F9'
#define U_TREM_MIN L'\x00FC'
#define U_CIRC_MIN L'\x00FB'

#define E_AIGU_MAJ L'\x00C9'
#define E_GRAV_MAJ L'\x00C8'
#define E_TREM_MAJ L'\x00CB'
#define E_CIRC_MAJ L'\x00CA'

#define A_AIGU_MAJ L'\x00C1'
#define A_GRAV_MAJ L'\x00C0'
#define A_TREM_MAJ L'\x00C4'
#define A_CIRC_MAJ L'\x00C2'

#define I_AIGU_MAJ L'\x00CD'
#define I_GRAV_MAJ L'\x00CC'
#define I_TREM_MAJ L'\x00CF'
#define I_CIRC_MAJ L'\x00CE'

#define O_AIGU_MAJ L'\x00D3'
#define O_GRAV_MAJ L'\x00D2'
#define O_TREM_MAJ L'\x00D6'
#define O_CIRC_MAJ L'\x00D4'

#define U_AIGU_MAJ L'\x00DA'
#define U_GRAV_MAJ L'\x00D9'
#define U_TREM_MAJ L'\x00DC'
#define U_CIRC_MAJ L'\x00DB'

#define C_CEDI_MIN L'\x00E7'
#define C_CEDI_MAJ L'\x00C7'

#define estEMinAccent(c) (((c) == E_AIGU_MIN) || ((c) == E_GRAV_MIN) ||((c) == E_TREM_MIN) ||((c) == E_CIRC_MIN))

#define estEMajAccent(c) (((c) == E_AIGU_MAJ) || ((c) == E_GRAV_MAJ) ||((c) == E_TREM_MAJ) ||((c) == E_CIRC_MAJ))

#define estEAccent(c) (estEMinAccent(c) || estEMajAccent(c))

#define estAMinAccent(c) (((c) == A_AIGU_MIN) || ((c) == A_GRAV_MIN) ||((c) == A_TREM_MIN) ||((c) == A_CIRC_MIN))

#define estAMajAccent(c) (((c) == A_AIGU_MAJ) || ((c) == A_GRAV_MAJ) ||((c) == A_TREM_MAJ) ||((c) == A_CIRC_MAJ))

#define estAAccent(c) (estAMinAccent(c) || estAMajAccent(c))

#define estIMinAccent(c) (((c) == I_AIGU_MIN) || ((c) == I_GRAV_MIN) ||((c) == I_TREM_MIN) ||((c) == I_CIRC_MIN))

#define estIMajAccent(c) (((c) == I_AIGU_MAJ) || ((c) == I_GRAV_MAJ) ||((c) == I_TREM_MAJ) ||((c) == I_CIRC_MAJ))

#define estIAccent(c) (estIMinAccent(c) || estIMajAccent(c))

#define estOMinAccent(c) (((c) == O_AIGU_MIN) || ((c) == O_GRAV_MIN) ||((c) == O_TREM_MIN) ||((c) == O_CIRC_MIN))

#define estOMajAccent(c) (((c) == O_AIGU_MAJ) || ((c) == O_GRAV_MAJ) ||((c) == O_TREM_MAJ) ||((c) == O_CIRC_MAJ))

#define estOAccent(c) (estOMinAccent(c) || estOMajAccent(c))

#define estUMinAccent(c) (((c) == U_AIGU_MIN) || ((c) == U_GRAV_MIN) ||((c) == U_TREM_MIN) ||((c) == U_CIRC_MIN))

#define estUMajAccent(c) (((c) == U_AIGU_MAJ) || ((c) == U_GRAV_MAJ) ||((c) == U_TREM_MAJ) ||((c) == U_CIRC_MAJ))

#define estUAccent(c) (estUMinAccent(c) || estUMajAccent(c))

#define estCCedille(c) ((c) == C_CEDI_MIN || (c) == C_CEDI_MAJ)

#define estSeparateur(c) (((c) == L'-') || ((c) == L'_'))

#endif
