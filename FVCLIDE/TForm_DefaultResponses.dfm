object Form_DefaultResponses: TForm_DefaultResponses
  Caption = 'Réponses par défaut'
  Left = 296
  Top = 246
  Width = 436
  Height = 236
  Constraints.MinWidth = 436
  Constraints.MinHeight = 236
  Position = poMainFormCenter
  object ListBox_DefaultResponses: TListBox
    Left = 8
    Top = 4
    Width = 340
    Height = 200
    Anchors = [akLeft, akTop, akRight, akBottom]
    OnDblClick = Button_ModifClick
    TabStop = True
    TabOrder = 0
  end
  object Button_MAZ: TButton
    Left = 356
    Top = 4
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Mise à zéro'
    TabStop = True
    TabOrder = 1
    OnClick = Button_MAZClick
  end
  object Button_Suppr: TButton
    Left = 356
    Top = 32
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Suppression'
    TabStop = True
    TabOrder = 2
    OnClick = Button_SupprClick
  end
  object Button_Sortie: TButton
    Left = 356
    Top = 60
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Sortie'
    TabStop = True
    TabOrder = 3
    Cancel = True
    OnClick = Button_SortieClick
  end
  object Button_Aide: TButton
    Left = 356
    Top = 88
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Aide'
    TabStop = True
    TabOrder = 4
    OnClick = Button_AideClick
  end
end

