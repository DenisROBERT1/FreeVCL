object Form_CreateProject: TForm_CreateProject
  Caption = 'Création d'#39'une nouvelle application'
  Left = 4
  Top = 0
  Width = 518
  Height = 274
  Position = poOwnerFormCenter
  Constraints.MinWidth = 518
  Constraints.MinHeight = 274
  object Label_Name: TLabel
    Caption = 'Nom de l'#39'application :'
    Left = 6
    Top = 10
    Width = 120
    Height = 13
  end
  object Edit_Name: TEdit
    Left = 132
    Top = 7
    Width = 372
    Height = 20
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 0
  end
  object Label_Path: TLabel
    Caption = 'Emplacement :'
    Left = 6
    Top = 39
    Width = 120
    Height = 13
  end
  object Edit_Path: TEdit
    Left = 132
    Top = 36
    Width = 306
    Height = 20
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 1
  end
  object Button_ParcourirPath: TButton
    Caption = 'Parcourir'
    Left = 444
    Top = 36
    Width = 60
    Height = 20
    Anchors = [akTop, akRight]
    OnClick = Button_ParcourirPathClick
    TabStop = False
  end
  object Label_Type: TLabel
    Caption = 'Type d'#39'application :'
    Left = 6
    Top = 68
    Width = 120
    Height = 13
  end
  object ComboBox_Type: TComboBox
    Left = 132
    Top = 65
    Width = 372
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 2
    Style = csDropDownList
    Items.Strings = (
		'Projet vide')
	ItemIndex = 1
  end
  object Button_Create: TButton
    Caption = 'Créer'
    Left = 150
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 3
    Default = True
    ModalResult = mrOk
  end
  object Button_Annuler: TButton
    Caption = 'Annuler'
    Left = 225
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 4
    ModalResult = mrCancel
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 300
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
    TabStop = True
    TabOrder = 5
    OnClick = Button_AideClick
  end
end
