object Form_SaisieColors: TForm_SaisieColors
  Left = 207
  Top = 107
  AutoScroll = False
  Caption = 'Paramètres de coloration syntaxique'
  Width = 560
  Height = 366
  Color = clBtnFace
  Constraints.MinWidth = 560
  Constraints.MinHeight = 366
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  DesignSize = (
    693
    433)
  PixelsPerInch = 96
  TextHeight = 13
  object Label_Categories: TLabel
    Left = 4
    Top = 4
    Width = 100
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Catégories :'
  end
  object ListBox_Categories: TListBox
    Left = 4
    Top = 24
    Width = 200
    Height = 136
    Items.Strings = (
      'Texte normal'
      'Texte sélectionné'
      'Mots clés'
      'Instructions préprocesseur'
      'Nombres'
      'Chaînes'
      'Commentaires'
      'Assembleur'
      'Numéros de lignes'
      'Icônes d'#39'état')
    TabStop = True
    TabOrder = 0
    ItemIndex = 0
    OnChange = ListBox_CategoriesChange
  end
  object Label_ColorText: TLabel
    Left = 4
    Top = 170
    Width = 40
    Height = 13
    Caption = 'Texte :'
  end
  object ColorBox_ColorText: TColorBox
    Left = 56
    Top = 166
    Width = 148
    Height = 100
    Style = [cbStandardColors, cbExtendedColors, cbSystemColors, cbCustomColor, cbPrettyNames]
    TabStop = True
    TabOrder = 2
    OnChange = ColorBox_ColorTextChange
  end
  object Label_ColorBack: TLabel
    Left = 4
    Top = 194
    Width = 40
    Height = 13
    Caption = 'Fond :'
  end
  object ColorBox_ColorBack: TColorBox
    Left = 56
    Top = 190
    Width = 148
    Height = 100
    Style = [cbStandardColors, cbExtendedColors, cbSystemColors, cbCustomColor, cbPrettyNames]
    TabStop = True
    TabOrder = 1
    OnChange = ColorBox_ColorBackChange
  end
  object CheckBox_Bold: TCheckBox
    Left = 4
    Top = 218
    Width = 64
    Height = 13
    Caption = 'Gras'
    Alignment = taLeftJustify
    TabStop = True
    TabOrder = 3
    OnClick = CheckBox_BoldClick
  end
  object CheckBox_Italic: TCheckBox
    Left = 4
    Top = 238
    Width = 64
    Height = 13
    Caption = 'Italique'
    Alignment = taLeftJustify
    TabStop = True
    TabOrder = 4
    OnClick = CheckBox_ItalicClick
  end
  object Label_Standard: TLabel
    Left = 4
    Top = 260
    Width = 48
    Height = 13
    Caption = 'Standard'
  end
  object ComboBox_Standard: TComboBox
    Left = 56
    Top = 258
    Width = 148
    Height = 100
    Items.Strings = (
      'FreeVCL Défaut'
      'FreeVCL Fond noir'
      'Builder'
      'Visual'
      'Code::Blocks')
    TabStop = True
    TabOrder = 5
    OnChange = ComboBox_StandardChange
  end
  object Label_Apercu: TLabel
    Left = 212
    Top = 4
    Width = 100
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = 'Aperçu :'
  end
  object Label_Langage: TLabel
    Caption = 'Langage :'
    Left = 400
    Top = 4
    Width = 68
    Height = 14
    Anchors = [akTop, akRight]
  end
  object ComboBox_Langage: TComboBox
    Left = 458
    Top = 2
    Width = 80
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 6
    Items.Strings = (
		'cpp'
		'cs'
		'pas'
		'vbs'
		'cbl'
		'gpp'
		'html')
	ItemIndex = 0;
	OnChange = ComboBox_LangageChange
  end
  object Frame_EditFichierText: TFrame_EditFichierText
    Left = 212
    Top = 24
    Width = 326
    Height = 256
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabStop = False
  end
  object Button_Ok: TButton
    Left = 144
    Top = 298
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Ok'
    ModalResult = mrOk
    TabOrder = 7
    Default = True
  end
  object Button_Annuler: TButton
    Left = 240
    Top = 298
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Annuler'
    ModalResult = mrCancel
    TabOrder = 8
    Cancel = True
  end
  object Button_Aide: TButton
    Left = 336
    Top = 298
    Width = 72
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Aide'
    TabOrder = 9
    OnClick = Button_AideClick
  end
end
