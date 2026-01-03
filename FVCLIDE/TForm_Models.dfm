object Form_Models: TForm_Models
  Caption = 'Modèles de code'
  Left = 296
  Top = 246
  Width = 436
  Height = 236
  Constraints.MinWidth = 436
  Constraints.MinHeight = 236
  Position = poMainFormCenter
  object DirectoryListBox_Models: TDirectoryListBox
    Left = 8
    Top = 4
    Width = 340
    Height = 200
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabStop = True
    TabOrder = 0
    ShowFiles = True
    OnChange = DirectoryListBox_Models_Change
    OnDblClick = Button_InsertClick
  end
  object Button_Insert: TButton
    Caption = 'Taper'
    Left = 356
    Top = 4
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 1
    Default = True
    OnClick = Button_InsertClick
  end
  object Button_Nouveau: TButton
    Caption = 'Ajout'
    Left = 356
    Top = 40
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 2
    OnClick = Button_NouveauClick
  end
  object Button_Modif: TButton
    Caption = 'Modification'
    Left = 356
    Top = 68
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 3
    OnClick = Button_ModifClick
  end
  object Button_Suppr: TButton
    Caption = 'Suppression'
    Left = 356
    Top = 96
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 4
    OnClick = Button_SupprClick
  end
  object Button_Sortie: TButton
    Caption = 'Sortie'
    Left = 356
    Top = 124
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 5
    Cancel = True
    OnClick = Button_SortieClick
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 356
    Top = 152
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 6
    OnClick = Button_AideClick
  end
end

