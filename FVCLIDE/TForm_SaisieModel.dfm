object Form_SaisieModel: TForm_SaisieModel
  Caption = 'Modèle de code'
  Left = 460
  Top = 230
  Width = 600
  Height = 400
  Constraints.MinWidth = 500
  Constraints.MinHeight = 300
  Position = poOwnerFormCenter
  object Label_Name: TLabel
    Caption = 'Nom :'
    Left = 8
    Top = 10
    Width = 68
    Height = 14
  end
  object Edit_Name: TEdit
    Left = 80
    Top = 6
    Width = 236
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 0
  end
  object Label_Langage: TLabel
    Caption = 'Langage :'
    Left = 340
    Top = 10
    Width = 58
    Height = 14
    Anchors = [akTop, akRight]
  end
  object ComboBox_Langage: TComboBox
    Left = 402
    Top = 6
    Width = 80
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 1
    Items.Strings = (
      'cpp'
      'cs'
      'pas'
      'vbs'
      'cbl'
			'gpp')
		OnChange = ComboBox_LangageChange
  end
  object Label_Model: TLabel
    Caption = 'Modèle :'
    Left = 8
    Top = 34
    Width = 68
    Height = 14
  end
  object Panel: TPanel
    Left = 8
    Top = 52
    Width = 576
    Height = 268
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabStop = True
    TabOrder = 2
    BevelOuter = bvLowered
    object Frame_EditFichierText: TFrame_EditFichierText
      Left = 0
      Top = 0
      Width = 572
      Height = 260
      Align = alClient
      TabStop = True
      TabOrder = 0
    end
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 174
    Top = 336
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    OnClick = Button_OkClick
    TabStop = True
    TabOrder = 3
    Default = True
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 262
    Top = 336
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 4
    Cancel = True
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 350
    Top = 336
    Width = 68
    Height = 23
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 5
    OnClick = Button_AideClick
  end
end
