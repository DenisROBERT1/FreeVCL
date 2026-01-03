object Form_Tools: TForm_Tools
  Caption = 'Liste des outils'
  Left = 296
  Top = 246
  Width = 436
  Height = 236
  Constraints.MinWidth = 436
  Constraints.MinHeight = 236
  Position = poMainFormCenter
  object ListBox_Tools: TListBox
    Left = 8
    Top = 4
    Width = 340
    Height = 200
    Anchors = [akLeft, akTop, akRight, akBottom]
    OnDblClick = Button_ModifClick
    TabStop = True
    TabOrder = 0
  end
  object Button_Nouveau: TButton
    Left = 356
    Top = 4
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Ajout'
    TabStop = True
    TabOrder = 1
    OnClick = Button_NouveauClick
  end
  object Button_Modif: TButton
    Left = 356
    Top = 32
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Modification'
    TabStop = True
    TabOrder = 2
    OnClick = Button_ModifClick
  end
  object Button_Suppr: TButton
    Left = 356
    Top = 60
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Suppression'
    TabStop = True
    TabOrder = 3
    OnClick = Button_SupprClick
  end
  object Button_Sortie: TButton
    Left = 356
    Top = 88
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Sortie'
    TabStop = True
    TabOrder = 4
    Cancel = True
    OnClick = Button_SortieClick
  end
  object Button_Aide: TButton
    Left = 356
    Top = 116
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Aide'
    TabStop = True
    TabOrder = 5
    OnClick = Button_AideClick
  end
end

