object Form_ChercherRemplacer: TForm_ChercherRemplacer
  Left = 327
  Top = 168
  Width = 428
  Height = 462
  BorderIcons = [biSystemMenu]
  Caption = 'Chercher et remplacer un texte'
  Color = clBtnFace
  AlphaBlend = True
  AlphaBlendValue = 31
  Constraints.MinWidth = 428
  Constraints.MinHeight = 462
  Constraints.MaxHeight = 462
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  OnClose = FormClose
  object TabControl: TTabControl
    Left = 0
    Top = 4
    Width = 420
    Height = 374
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    Tabs.Strings = (
      'Chercher'
      'Chercher dans des fichiers'
      'Remplacer'
      'Remplacer dans des fichiers')
    TabIndex = 0
    OnChange = TabControlChange
    object Label_Chercher: TLabel
      Left = 6
      Top = 36
      Width = 74
      Height = 13
      Caption = 'Chercher :'
    end
    object ComboBox_Chercher: TComboBox
      Left = 82
      Top = 32
      Width = 332
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 0
    end
    object Label_Remplacer: TLabel
      Left = 6
      Top = 62
      Width = 74
      Height = 13
      Caption = 'Remplacer par :'
      Visible = False
    end
    object ComboBox_Remplacer: TComboBox
      Left = 82
      Top = 58
      Width = 332
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      TabOrder = 1
      Visible = False
    end
    object GroupBox_Options: TGroupBox
      Left = 6
      Top = 86
      Width = 202
      Height = 220
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Options'
      TabStop = True
      TabOrder = 2
      object CheckBox_MotEntier: TCheckBox
        Left = 8
        Top = 16
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Mot entier'
        TabOrder = 0
      end
      object CheckBox_MAJmin: TCheckBox
        Left = 8
        Top = 36
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Différentier MAJ/min'
        TabOrder = 1
      end
      object CheckBox_DebutLigne: TCheckBox
        Left = 8
        Top = 56
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'En début de ligne'
        TabOrder = 2
      end
      object CheckBox_FinLigne: TCheckBox
        Left = 8
        Top = 76
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'En fin de ligne'
        TabOrder = 3
      end
      object CheckBox_DansCommentaires: TCheckBox
        Left = 8
        Top = 96
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Uniquement dans les commentaires'
        TabOrder = 4
        OnClick = CheckBox_DansCommentairesClick
      end
      object CheckBox_HorsCommentaires: TCheckBox
        Left = 8
        Top = 116
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Uniquement hors des commentaires'
        TabOrder = 5
        OnClick = CheckBox_HorsCommentairesClick
      end
      object CheckBox_DansGuillemets: TCheckBox
        Left = 8
        Top = 136
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Uniquement dans les guillemets'
        TabOrder = 6
        OnClick = CheckBox_DansGuillemetsClick
      end
      object CheckBox_HorsGuillemets: TCheckBox
        Left = 8
        Top = 156
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Uniquement hors des guillemets'
        TabOrder = 7
        OnClick = CheckBox_HorsGuillemetsClick
      end
      object CheckBox_ExpressionReg: TCheckBox
        Left = 8
        Top = 176
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Expression régulière'
        TabOrder = 8
      end
      object CheckBox_ConserverCasse: TCheckBox
        Left = 8
        Top = 196
        Width = 188
        Height = 17
	    Anchors = [akLeft, akTop, akRight]
        Caption = 'Conserver la casse'
        TabOrder = 9
      end
    end
    object GroupBox_ChercherDans: TGroupBox
      Left = 212
      Top = 86
      Width = 202
      Height = 220
      Anchors = [akTop, akRight]
      Caption = 'Chercher dans'
      TabStop = True
      TabOrder = 3
      Visible = False
      object RadioButton_FichiersProjet: TRadioButton
        Left = 8
        Top = 16
        Width = 188
        Height = 17
        Caption = 'Fichiers du projet'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = RadioButton_FichiersProjetClick
      end
      object RadioButton_FichiersTousProjet: TRadioButton
        Left = 8
        Top = 36
        Width = 188
        Height = 17
        Caption = 'Fichiers de tous les projets'
        TabOrder = 1
        TabStop = True
        OnClick = RadioButton_FichiersTousProjetClick
      end
      object RadioButton_FichiersOuverts: TRadioButton
        Left = 8
        Top = 56
        Width = 188
        Height = 17
        Caption = 'Fichiers ouverts'
        TabOrder = 2
        OnClick = RadioButton_FichiersOuvertsClick
      end
      object RadioButton_Repertoire: TRadioButton
        Left = 8
        Top = 76
        Width = 188
        Height = 17
        Caption = 'Répertoire'
        TabOrder = 3
        OnClick = RadioButton_RepertoireClick
      end
      object CheckBox_SousRep: TCheckBox
        Left = 24
        Top = 96
        Width = 100
        Height = 17
        Caption = 'Sous-répertoires'
        Enabled = False
        TabOrder = 4
      end
    end
    object GroupBox_Direction: TGroupBox
      Left = 212
      Top = 86
      Width = 202
      Height = 64
      Anchors = [akTop, akRight]
      Caption = 'Direction'
      TabStop = True
      TabOrder = 4
      object RadioButton_VersLeHaut: TRadioButton
        Left = 8
        Top = 16
        Width = 188
        Height = 17
        Caption = 'Vers le haut'
        TabOrder = 0
      end
      object RadioButton_VersLeBas: TRadioButton
        Left = 8
        Top = 36
        Width = 188
        Height = 17
        Caption = 'Vers le bas'
        Checked = True
        TabOrder = 1
        TabStop = True
      end
    end
    object GroupBox_Origine: TGroupBox
      Left = 212
      Top = 154
      Width = 202
      Height = 84
      Anchors = [akTop, akRight]
      Caption = 'Origine'
      TabStop = True
      TabOrder = 5
      object RadioButton_DepuisDebut: TRadioButton
        Left = 8
        Top = 16
        Width = 188
        Height = 17
        Caption = 'Depuis le début'
        Checked = True
        TabOrder = 0
        TabStop = True
	    OnClick = RadioButton_DepuisDebutClick
      end
      object RadioButton_DepuisCurseur: TRadioButton
        Left = 8
        Top = 36
        Width = 188
        Height = 17
        Caption = 'À partir du curseur'
        TabOrder = 1
	    OnClick = RadioButton_DepuisCurseurClick
      end
      object CheckBox_Boucler: TCheckBox
        Left = 8
        Top = 56
        Width = 188
        Height = 17
        Caption = 'Boucler en fin de fichier'
        TabOrder = 2
      end
    end
    object GroupBox_Portee: TGroupBox
      Left = 212
      Top = 242
      Width = 202
      Height = 64
      Anchors = [akTop, akRight]
      Caption = 'Portée'
      TabStop = True
      TabOrder = 6
      object RadioButton_Globale: TRadioButton
        Left = 8
        Top = 16
        Width = 188
        Height = 17
        Caption = 'Dans tout le texte'
        Checked = True
        TabOrder = 0
        TabStop = True
      end
      object RadioButton_TexteSelectionne: TRadioButton
        Left = 8
        Top = 36
        Width = 188
        Height = 17
        Caption = 'Dans le texte sélectionné'
        TabOrder = 1
      end
    end
    object Label_Repertoire: TLabel
      Left = 6
      Top = 316
      Width = 64
      Height = 13
      Caption = 'Répertoire'
      Enabled = False
      Visible = False
    end
    object Edit_Repertoire: TEdit
      Left = 72
      Top = 314
      Width = 312
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      TabOrder = 6
      Visible = False
    end
    object Button_Repertoire: TButton
      Left = 390
      Top = 314
      Width = 24
      Height = 21
      Anchors = [akTop, akRight]
      Caption = '...'
      Enabled = False
      TabOrder = 7
      Visible = False
      OnClick = Button_RepertoireClick
    end
    object Label_Extensions: TLabel
      Left = 6
      Top = 342
      Width = 64
      Height = 13
      Caption = 'Extensions'
      Enabled = False
      Visible = False
    end
    object ComboBox_Extensions: TComboBox
      Left = 72
      Top = 340
      Width = 342
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      Style = csDropDown
      Items.Strings = (
		'*.c;*.cpp;*.h;'
		'*.cs;'
		'*.pas;*.hpp;'
		'*.bas;'
		'*.gpp;')
      Enabled = False
      TabOrder = 8
      Visible = False
    end
  end
  object Button_Chercher: TButton
    Left = 10
    Top = 396
    Width = 70
    Height = 24
    Caption = 'Chercher'
    Default = True
    TabOrder = 1
    OnClick = Button_ChercherClick
  end
  object Button_Remplacer: TButton
    Left = 90
    Top = 396
    Width = 70
    Height = 24
    Caption = 'Remplacer'
    TabOrder = 2
    Enabled = False
    OnClick = Button_RemplacerClick
  end
  object Button_Tout: TButton
    Left = 170
    Top = 396
    Width = 70
    Height = 24
    Caption = 'Tout'
    TabOrder = 3
    OnClick = Button_ToutClick
  end
  object Button_Annuler: TButton
    Left = 250
    Top = 396
    Width = 70
    Height = 24
    Cancel = True
    Caption = 'Sortie'
    ModalResult = mrCancel
    TabOrder = 4
  end
  object Button_Aide: TButton
    Left = 330
    Top = 396
    Width = 70
    Height = 24
    Caption = 'Aide'
    TabOrder = 5
    OnClick = Button_AideClick
  end
  object ProgressBar: TProgressBar
    Left = 10
    Top = 388
    Width = 320
    Height = 21
    Anchors = [akLeft, akTop, akRight]
		Smooth = True
		Visible = False
  end
  object Button_Abandon: TButton
    Left = 340
    Top = 388
    Width = 70
    Height = 21
    Anchors = [akTop, akRight]
    Caption = 'Arrêter'
    TabOrder = 6
		Visible = False
		OnClick = Button_AbandonClick
  end
  object Label_Progress: TLabel
    Left = 10
    Top = 412
    Width = 400
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = ''
		Visible = False
  end
  object Timer_AlphaBlend: TTimer
    Interval = 50
    Enabled = True
		OnTimer = Timer_AlphaBlendTimer
  end
end
