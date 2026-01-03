object Form_AddNewFile: TForm_AddNewFile
  Caption = 'Ajouter un nouveau fichier'
  Left = 296
  Top = 246
  Width = 436
  Height = 300
  Constraints.MinWidth = 436
  Constraints.MinHeight = 300
  Position = poMainFormCenter
  object Label_Name: TLabel
    Caption = 'Nom :'
    Left = 8
    Top = 6
    Width = 70
    Height = 13
  end
  object Edit_Name: TEdit
    Left = 88
    Top = 4
    Width = 332
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 0
  end
  object Label_Path: TLabel
    Caption = 'Emplacement :'
    Left = 8
    Top = 28
    Width = 70
    Height = 13
  end
  object Edit_Path: TEdit
    Left = 88
    Top = 26
    Width = 268
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 1
  end
  object Button_ParcourirPath: TButton
    Caption = 'Parcourir'
    Left = 360
    Top = 26
    Width = 60
    Height = 20
    Anchors = [akTop, akRight]
    OnClick = Button_ParcourirPathClick
    TabStop = False
  end
  object Label_Types: TLabel
    Caption = 'Type de fichier à ajouter :'
    Left = 8
    Top = 56
    Width = 412
    Height = 13
  end
  object ListBox_Types: TListBox
    Left = 8
    Top = 76
    Width = 412
    Height = 142
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemIndex = 0
    TabStop = True
    TabOrder = 2
  end
  object Button_Ok: TButton
    Caption = 'Ok'
    Left = 92
    Top = 238
    Width = 68
    Height = 22
    Anchors = [akLeft, akBottom]
    OnClick = Button_OkClick
    TabStop = True
    TabOrder = 3
    Default = True
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 180
    Top = 238
    Width = 68
    Height = 22
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 4
	ModalResult = mrCancel
    Cancel = True
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 268
    Top = 238
    Width = 68
    Height = 22
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 5
    OnClick = Button_AideClick
  end
end
