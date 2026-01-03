//-----------------------------------------------------------------------------
//! @file TDBFFile.h
//!
//! @author Denis ROBERT (http://denis.beru.free.fr)
//!
//! Copyright 2008-2025
//! @n
//! This file is part of FreeVCL.
//! @n
//!     FreeVCL is free software: you can redistribute it and/or modify
//!     it under the terms of the GNU Lesser General Public License as
//!     published by the Free Software Foundation, either version 3 of
//!     the License, or (at your option) any later version.
//! @n
//!     FreeVCL is distributed in the hope that it will be useful,
//!     but WITHOUT ANY WARRANTY; without even the implied warranty of
//!     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//!     GNU Lesser General Public License for more details.
//! @n
//!     You should have received a copy of the GNU Lesser General Public
//!     License along with FreeVCL.
//!     If not, see <http://www.gnu.org/licenses/>.
//!
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Dispositif anti-redéfinition
//-----------------------------------------------------------------------------

#ifdef In_TDBFFileH
#error "Inclusion circulaire de TDBFFile"
#endif // In_TDBFFileH

#define In_TDBFFileH

#ifndef TDBFFileH
#define TDBFFileH

#include <vector>

//---------------------------------------------------------------------------
// Structures
//---------------------------------------------------------------------------

#pragma pack(push, 1)

typedef struct {
  BYTE ValidDBASE;                     // = 03h sans fichier memo DBT, 83h avec memo
  BYTE YY;                             //
  BYTE MM;                             // Date de dernière modification
  BYTE DD;                             //
  long nbRecords;                      // Nombre d'enregistrements
  short LengthHeader;                  // Longueur de l'en-tête
  short LengthRecord;                  // Longueur d'un enregistrement
  BYTE Reserved1[3];
  BYTE Reserved2[13];
  // BYTE Reserved3[4];                // Remplacé par "code secret"
  unsigned long SecretCode;            // Code secret (?)
} HEADERDBF, *LPHEADERDBF;

//---------------------------------------------------------------------------

typedef struct {
  BYTE ValidDBASE;                     // = 04h sans fichier memo DBT, 84h avec memo
  BYTE YY;                             //
  BYTE MM;                             // Date de dernière modification
  BYTE DD;                             //
  long nbRecords;                      // Nombre d'enregistrements
  short LengthHeader;                  // Longueur de l'en-tête
  short LengthRecord;                  // Longueur d'un enregistrement
  BYTE Reserved1[2];
  BYTE IncompleteTrans;
  BYTE EncryptionFlag;
  BYTE Reserved2[12];
  BYTE ProductionMDX;
  BYTE LangageDriverID;
  BYTE Reserved3[2];
  char LangageDriverName[32];
  // BYTE Reserved4[4];                // Remplacé par "code secret"
  unsigned long SecretCode;            // Code secret (pas sûr qu'il soit à cette position)
} HEADERDBF4, *LPHEADERDBF4;

//---------------------------------------------------------------------------

typedef struct {
  char FieldName[11];                  // Nom du champ
  BYTE FieldType;                      // C, D, L, M ou N
  long FieldDataAdress;                //
  BYTE FieldLength;                    // Longueur du champ
  BYTE FieldDecimal;                   // Nombre de décimales
  BYTE Reserved1[2];
  BYTE WorkAreaId;
  BYTE Reserved2[2];
  BYTE SetField;
  BYTE Reserved3[8];
} FIELDDBF, *LPFIELDDBF;

//---------------------------------------------------------------------------

typedef struct {
  char FieldName[32];                  // Nom du champ
  BYTE FieldType;                      // B, C, D, N, L, M, @, I, +, F, O ou G
  BYTE FieldLength;                    // Longueur du champ
  BYTE FieldDecimal;                   // Nombre de décimales
  BYTE Reserved1[2];
  BYTE ProductionMDX;
  BYTE Reserved2[2];
  long AutoIncrement;                  // Prochaine valeur d'auto-incrémentation
  long FieldPos;
} FIELDDBF4, *LPFIELDDBF4;

#pragma pack(pop)


//---------------------------------------------------------------------------
//! @class TDBFFile
//!
//! @brief Fichier DBF
//!
//! Cette classe (@c TDBFFile) représente un fichier DBF.
//!
//! @author Denis ROBERT
//-----------------------------------------------------------------------------

class TDBFFile: public TComponent {
private:

protected:
  HANDLE hFile;
  int Version;
  HEADERDBF4 HeaderDbf;
  std::vector <FIELDDBF4> FFields;
  int DebData;
  BYTE FWorkAreaId;

  char *Record;
  char *Data;
  int NumRecord;
  bool bModifRecord;
  bool bModifFile;

  //---------------------------------------------------------------------------
  //! @brief Lecture d'un enregistrement
  //!
  //! Cette méthode permet de lire un enregistrement (méthode interne)
  //!
  //! @param[in]        NumEnr Numéro d'enregistrement à lire
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------

  bool ReadRecord(int NumEnr);

  //---------------------------------------------------------------------------
  //! @brief Ecriture de l'enregistrement courant
  //!
  //! Cette méthode permet d'écrire un enregistrement (méthode interne) avant
  //! d'en lire un autre ou de fermer le fichier.
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------

  bool WriteRecord();

  //---------------------------------------------------------------------------
  //! @brief Ecriture de l'entête
  //!
  //! Cette méthode permet d'écrire le header et les fields du fichier
  //!
  //! @return @c bool True si Ok
  //---------------------------------------------------------------------------

  bool WriteHeader();

public:

	//! @name Constructeurs et destructeur
  //@{

  //---------------------------------------------------------------------------
  //! @brief Constructeur
  //---------------------------------------------------------------------------

  TDBFFile(TComponent *AOwner);


  //---------------------------------------------------------------------------
  //! @brief Destructeur
  //---------------------------------------------------------------------------

  virtual ~TDBFFile();

  //@}


  //! @name Méthodes
  //@{

  //---------------------------------------------------------------------------
  //! @brief Création d'un fichier DBF
  //!
  //! Cette fonction crée un fichier DBF vide. En général, vous aurez à créer
  //! ensuite les champs. Si un fichier de même nom existe déjà, il sera
  //! détruit.
  //!
  //! @param[in]        asFileName Nom du fichier à créer
  //!
  //! @return @c bool true si OK. Sinon une erreur est affichée.
  //---------------------------------------------------------------------------
  bool CreateFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Ouverture d'un fichier DBF
  //!
  //! Cette fonction ouvre un fichier DBF en lecture seule.
  //!
  //! @param[in]        asFileName Nom du fichier à ouvrir.
  //!
  //! @return @c bool true si Ok. Sinon une erreur est affichée.
  //---------------------------------------------------------------------------
  bool OpenFile(AnsiString asFileName);

  //---------------------------------------------------------------------------
  //! @brief Ferme un fichier.
  //!
  //! Cette fonction ferme un fichier DBF préalablement ouvert.
  //!
  //! @return @c bool true si Ok. Sinon une erreur est affichée. Cette méthode
  //! n'affiche pas d'erreur si on essaie de fermer un fichier déjà fermé ou
  //! jamais ouvert.
  //---------------------------------------------------------------------------
  bool CloseFile();

  //---------------------------------------------------------------------------
  //! @brief Lecture d'une donnée
  //!
  //! Cette fonction lit une donnée du fichier DBF.
  //!
  //! @param[in]        NumEnr Numéro d'enregistrement à lire (premier = 0)
  //! @param[in]        NumField uméro de champ à lire (premier = 0)
  //!
  //! @return @c AnsiString Champ lu sous forme de chaîne.
  //---------------------------------------------------------------------------
  AnsiString GetData(int NumEnr, int NumField);

  //---------------------------------------------------------------------------
  //! @brief Ajout d'un champ dans un fichier DBF
  //!
  //! Cette fonction ajoute un champ dans le fichier DBF.
  //!
  //! @param[in]        asName Nom du champ à ajouter
  //! @param[in]        Type Type du champ (Type DBF: "c", "n", ...)
  //! @param[in]        Length Longueur du champ en octets
  //! @param[in]        Decimal Nombre de décimales si nombre réel (ignoré sinon).
  //! @param[in]        WorkAreaId Heu... Je sais pas.
  //!
  //! @return @c bool true si Ok.
  //!
  //! @warning Le fichier doit être vide pour pouvoir ajouter des champs.
  //---------------------------------------------------------------------------
  bool AddField(AnsiString asName, BYTE Type, int Length, int Decimal, int WorkAreaId);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une donnée dans le fichier DBF
  //!
  //! Cette fonction écrit une valeur entière dans le fichier DBF
  //!
  //! @param[in]        NumEnr Numéro d'enregistrement à écrire (premier = 0)
  //! @param[in]        NumField uméro de champ à écrire (premier = 0)
  //! @param[in]        Valeur Valeur à écrire
  //!
  //! @return @c bool true si Ok
  //!
  //! @note Si la valeur est écrite après la fin de fichier, les
  //! enregistrements intermédiaires sont mis à zéro.
  //---------------------------------------------------------------------------
  bool SetData(int NumEnr, int NumField, int Valeur);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une donnée dans le fichier DBF
  //!
  //! Cette fonction écrit une valeur de type date dans le fichier DBF
  //!
  //! @param[in]        NumEnr Numéro d'enregistrement à écrire (premier = 0)
  //! @param[in]        NumField uméro de champ à écrire (premier = 0)
  //! @param[in]        ValDate Date à écrire
  //!
  //! @return @c bool true si Ok
  //!
  //! @note Si la valeur est écrite après la fin de fichier, les
  //! enregistrements intermédiaires sont mis à zéro.
  //---------------------------------------------------------------------------
  bool SetData(int NumEnr, int NumField, TDateTime &ValDate);

  //---------------------------------------------------------------------------
  //! @brief Ecriture d'une donnée dans le fichier DBF
  //!
  //! Cette fonction écrit une valeur de type chaîne dans le fichier DBF
  //!
  //! @param[in]        NumEnr Numéro d'enregistrement à écrire (premier = 0)
  //! @param[in]        NumField uméro de champ à écrire (premier = 0)
  //! @param[in]        asValue Valeur à écrire
  //!
  //! @return @c bool true si Ok
  //!
  //! @note Si la valeur est écrite après la fin de fichier, les
  //! enregistrements intermédiaires sont mis à zéro.
  //---------------------------------------------------------------------------
  bool SetData(int NumEnr, int NumField, AnsiString asValue);

  //---------------------------------------------------------------------------
  //! @brief Liste des propriétés publiées
  //!
  //! @param[in, out]   ListProperties Liste des propriétés de l'objet
  //!
  //! @sa TPersistent::GetListProperties
  //---------------------------------------------------------------------------
  virtual void GetListProperties(TStrings *ListProperties);

  //---------------------------------------------------------------------------
  //! @brief Type des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété
  //! @param[out]       asInfos Liste des choix (si type = tpChoice ou tpMultipleChoice) ou arguments (si type = tpEvent)
  //!
  //! @return @c TYPEPROPERTY Type de la propriété concernée.
  //!
  //! @sa TPersistent::GetTypeProperty
  //---------------------------------------------------------------------------

  virtual TYPEPROPERTY GetTypeProperty(AnsiString asProperty, AnsiString *asInfos);

  //---------------------------------------------------------------------------
  //! @brief Valeur par défaut d'une propriété
  //!
  //! @param[in]        asProperty Nom de la propriété
  //!
  //! @return @c AnsiString Valeur par défaut de la propriété.
  //!
  //! @sa TPersistent::GetDefaultProperty
  //---------------------------------------------------------------------------

  virtual AnsiString GetDefaultProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Lecture des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à lire
  //!
  //! @return @c AnsiString Valeur de la propriété.
  //---------------------------------------------------------------------------

  virtual AnsiString GetProperty(AnsiString asProperty);

  //---------------------------------------------------------------------------
  //! @brief Affectation des propriétés publiées
  //!
  //! @param[in]        asProperty Nom de la propriété à enregistrer
  //! @param[in]        asValue Valeur de la propriété à enregistrer
  //! @param[in]        Sender Boîte de dialogue (utilisé par les évènements)
  //!
  //! @return @c bool True si Ok.
  //---------------------------------------------------------------------------

  virtual bool SetProperty(AnsiString asProperty, AnsiString asValue,
                           TPersistent *Sender);

  //@}


  //! @name Propriétés
  //@{

  //---------------------------------------------------------------------------
  //! @brief Propriété NbFields
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDBFFile, int, NbFields);


  //---------------------------------------------------------------------------
  //! @brief Propriété NbRecords
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_GET(TDBFFile, int, NbRecords);


  //---------------------------------------------------------------------------
  //! @brief Propriété FieldLength
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TDBFFile, int, FieldLength);


  //---------------------------------------------------------------------------
  //! @brief Propriété FieldType
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TDBFFile, char, FieldType);


  //---------------------------------------------------------------------------
  //! @brief Propriété FieldName
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY_TAB_GET(TDBFFile, AnsiString, FieldName);


  //---------------------------------------------------------------------------
  //! @brief Propriété DBFVersion
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDBFFile, BYTE, DBFVersion);

  //---------------------------------------------------------------------------
  //! @brief Propriété SecretCode
	//!
	//! Cette propriété ___INSERER_LA_DESCRIPTION___
  //---------------------------------------------------------------------------

  DECLARE_PROPERTY(TDBFFile, unsigned long, SecretCode);


  //@}
};

//---------------------------------------------------------------------------
#endif

#undef In_TDBFFileH
