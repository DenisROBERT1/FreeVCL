object Form_CreateProject: TForm_CreateProject
  Caption = 'Création d'#39'une nouvelle application'
  Left = 4
  Top = 0
  Width = 518
  Height = 274
  Constraints.MinWidth = 518
  Constraints.MinHeight = 274
  Position = poOwnerFormCenter
  object Label_Name: TLabel
    Caption = 'Nom de l'#39'application'
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
    TabOrder = 0
  end
  object Label_Path: TLabel
    Caption = 'Emplacement'
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
  object GroupBox_Creation: TGroupBox
    Caption = 'Création'
    Left = 4
    Top = 62
    Width = 500
    Height = 134
    Anchors = [akLeft, akTop, akRight, akBottom]
    object RadioButton_Modele: TRadioButton
      Caption = 'A partir d'#39'un modèle'
      Left = 8
      Top = 20
      Width = 300
      Height = 20
      Anchors = [akLeft, akTop, akRight]
      OnClick = RadioButton_ModeleClick
      TabOrder = 0
      Checked = True
    end
    object Label_Modele: TLabel
      Caption = 'Modèle'
      Left = 28
      Top = 46
      Width = 114
      Height = 13
    end
    object ComboBox_Modele: TComboBox
      Left = 148
      Top = 43
      Width = 344
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      Style = csDropDownList
      Items.Strings = (
        'Projet vide'
        'Projet minimal de type "Hello world"'
        'Type éditeur (menu "Ouvrir", "Fermer", "Enregistrer", ...)'
      )
      ItemIndex = 1
    end
    object RadioButton_AppliVCL: TRadioButton
      Caption = 'A partir d'#39'une application VCL'
      Left = 8
      Top = 70
      Width = 296
      Height = 18
      Anchors = [akLeft, akTop, akRight]
      OnClick = RadioButton_AppliVCLClick
      TabOrder = 2
    end
    object Label_PathAppliVCL: TLabel
      Caption = 'Chemin de l'#39'application'
      Left = 28
      Top = 96
      Width = 112
      Height = 16
      Enabled = False
    end
    object Edit_PathAppliVCL: TEdit
      Left = 148
      Top = 94
      Width = 268
      Height = 20
      Anchors = [akLeft, akTop, akRight]
      Enabled = False
      TabOrder = 3
    end
    object Button_ParcourirPathAppliVCL: TButton
      Caption = 'Parcourir'
      Left = 420
      Top = 94
      Width = 72
      Height = 20
      Anchors = [akTop, akRight]
      Enabled = False
      OnClick = TButton_ParcourirPathAppliVCLClick
      TabOrder = 4
    end
  end
  object Button_Create: TButton
    Caption = 'Créer'
    Left = 150
    Top = 211
    Width = 60
    Height = 20
    Anchors = [akLeft, akBottom]
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
    OnClick = Button_AideClick
    TabOrder = 5
  end
end

