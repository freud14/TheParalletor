/**
        @file	Algorithm.cpp
                        Fichier contenant l'implémentation de la classe Algorithm.
        @author Frédérik Paradis
        @author Louis-Étienne Dorval
        @date   16 mai 2011
*/


#include "Algorithm.h"

/**
        Le constructeur initialise l'objet. La chaine prise en paramètre
        est copié dans l'attribut <code>hash</code> jusqu'à la taille <code>taille</code>.
        @param [in] hash Le hash
        @param [in] taille La taille du tableau hash
*/
Algorithm::Algorithm(unsigned char* hash, int taille)
{
        this->hash = new unsigned char[taille];
        for(int i = 0; i < taille; i++)
        {
                this->hash[i] = hash[i];
        }

        this->taille = taille;
}

/**
        Le destructeur libère les ressources prises par l'objet.
*/
Algorithm::~Algorithm(void)
{
        delete[] this->hash;
}

/**
        Cette fonction se charge de prendre une chaine de caractère
        représentant un hash en hexadécimal et la convertis en tableau
        de char. La taille de la chaîne de caractère doit être un multiple de 2.
        @param [in] mot La chaine de caractère représentant le hash en hexadécimal.
        @return Retourne le hash en format binaire.
*/
unsigned char* Algorithm::hashToHex(std::string mot)
{
        assert(mot.length() % 2 == 0);
        unsigned char * retour = new unsigned char[mot.length() / 2];
        int i = 0;
        for(std::string::iterator it =  mot.begin(); it != mot.end(); ++it)
        {
                char tmp[3] = {*it, *(++it), 0};
                char * pEnd;
                unsigned char res;
                res = strtol(tmp,&pEnd,16);
                retour[i] = res;
                i++;
        }

        return retour;
}
