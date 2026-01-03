object Form_Statistics: TForm_Statistics
  Left = 327
  Top = 168
  Width = 404
  Height = 412
  BorderIcons = [biSystemMenu]
  Caption = 'Statistiques'
  Color = clBtnFace
  Constraints.MinWidth = 404
  Constraints.MinHeight = 412
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
  object RadioButton_FichiersOuverts: TRadioButton
    Left = 8
    Top = 8
    Width = 200
    Height = 17
    TabOrder = 0
    Checked = True
    Caption = 'Sur les fichiers ouverts'
    OnClick = RadioButton_FichiersOuvertsClick
  end
  object RadioButton_FichiersProjet: TRadioButton
    Left = 8
    Top = 28
    Width = 200
    Height = 17
    TabOrder = 1
    Caption = 'Sur les fichiers du projet'
    OnClick = RadioButton_FichiersProjetClick
  end
  object RadioButton_Repertoire: TRadioButton
    Left = 8
    Top = 48
    Width = 200
    Height = 17
    TabOrder = 2
    Caption = 'Sur les fichiers d'#39'un répertoire :'
    OnClick = RadioButton_RepertoireClick
  end
  object CheckBox_SousRep: TCheckBox
    Left = 76
    Top = 68
    Width = 100
    Height = 17
    Caption = 'Sous-répertoires'
    Enabled = False
    TabOrder = 3
  end
  object Label_Repertoire: TLabel
    Left = 8
    Top = 92
    Width = 64
    Height = 13
    Caption = 'Répertoire'
    Enabled = False
  end
  object Edit_Repertoire: TEdit
    Left = 76
    Top = 90
    Width = 282
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Enabled = False
    TabOrder = 3
  end
  object Button_Repertoire: TButton
    Left = 364
    Top = 90
    Width = 24
    Height = 21
    Anchors = [akTop, akRight]
    Caption = '...'
    Enabled = False
    TabOrder = 2
    OnClick = Button_RepertoireClick
  end
  object Label_Extensions: TLabel
    Left = 8
    Top = 116
    Width = 64
    Height = 13
    Caption = 'Extensions'
    Enabled = False
  end
  object ComboBox_Extensions: TComboBox
    Left = 76
    Top = 114
    Width = 312
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
    TabOrder = 4
  end
  object Memo_Statistics: TMemo
    Left = 8
    Top = 140
    Width = 380
    Height = 168
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssNone
  end
  object CheckBox_Cumul: TCheckBox
    Left = 8
    Top = 314
    Width = 200
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = 'Cumuler les résultats'
    TabOrder = 5
  end
  object Button_Statistics: TButton
    Left = 54
    Top = 346
    Width = 79
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Statistiques'
    Default = True
    TabOrder = 6
    OnClick = Button_StatisticsClick
  end
  object Button_Sortie: TButton
    Left = 156
    Top = 346
    Width = 79
    Height = 25
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = 'Sortie'
    ModalResult = mrCancel
    TabOrder = 7
  end
  object Button_Aide: TButton
    Left = 258
    Top = 346
    Width = 79
    Height = 25
    Anchors = [akLeft, akBottom]
    Caption = 'Aide'
    TabOrder = 8
    OnClick = Button_AideClick
  end
  object ProgressBar: TProgressBar
    Left = 10
    Top = 338
    Width = 296
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
		Smooth = True
		Visible = False
  end
  object Button_Abandon: TButton
    Left = 316
    Top = 338
    Width = 70
    Height = 21
    Anchors = [akRight, akBottom]
    Caption = 'Arrêter'
    TabOrder = 9
		Visible = False
		OnClick = Button_AbandonClick
  end
  object Label_Progress: TLabel
    Left = 10
    Top = 362
    Width = 376
    Height = 13
    Anchors = [akLeft, akRight, akBottom]
    Caption = ''
		Visible = False
  end
end
