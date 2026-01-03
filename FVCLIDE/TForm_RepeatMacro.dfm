object Form_RepeatMacro: TForm_RepeatMacro
  Caption = 'Répéter une macro'
  Left = 292
  Top = 235
  Width = 273
  Height = 170
  Constraints.MinWidth = 273
  Constraints.MinHeight = 170
  Position = poMainFormCenter
  object TLabel_NbrRepet: TLabel
    Caption = 'Nombre de répétitions :'
    Left = 8
    Top = 12
    Width = 120
    Height = 12
  end
  object TEdit_NbrRepet: TEdit
    Left = 136
    Top = 8
    Width = 88
    Height = 20
    TabStop = True
    Text = ''
    TabOrder = 0
  end
  object TUpDown_NbrRepet: TUpDown
    Left = 228
    Top = 8
    Width = 16
    Height = 20
		Associate = 'TEdit_NbrRepet'
  end
  object TCheckBox_StopBegin: TCheckBox
    Caption = 'Arrêter en début de fichier'
    Left = 12
    Top = 40
    Width = 236
    Height = 16
    TabStop = True
    TabOrder = 1
  end
  object TCheckBox_StopEnd: TCheckBox
    Caption = 'Arrêter en fin de fichier'
    Left = 12
    Top = 64
    Width = 236
    Height = 16
    TabStop = True
    TabOrder = 2
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 10
    Top = 107
    Width = 68
    Height = 22
    TabStop = True
    TabOrder = 3
    Default = True
		ModalResult = mrOk
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 98
    Top = 107
    Width = 68
    Height = 22
    TabStop = True
    TabOrder = 4
    Cancel = True
		ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 186
    Top = 107
    Width = 68
    Height = 22
    OnClick = Button_AideClick
    TabStop = True
    TabOrder = 5
  end
end
