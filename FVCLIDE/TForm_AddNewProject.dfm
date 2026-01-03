object Form_AddNewProject: TForm_AddNewProject
  Caption = 'Ajouter un nouveau projet'
  Left = 296
  Top = 246
  Width = 436
  Height = 336
  Constraints.MinWidth = 436
  Constraints.MinHeight = 336
  Position = poMainFormCenter
  object Label_Name: TLabel
    Left = 8
    Top = 6
    Width = 50
    Height = 13
    Caption = 'Nom :'
  end
  object Edit_Name: TEdit
    Left = 62
    Top = 4
    Width = 286
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabStop = True
    TabOrder = 0
  end
  object Label_Types: TLabel
    Left = 8
    Top = 34
    Width = 340
    Height = 13
    Caption = 'Type de projet à ajouter :'
  end
  object ListBox_Types: TListBox
    Left = 8
    Top = 54
    Width = 340
    Height = 250
    Anchors = [akLeft, akTop, akRight, akBottom]
		Style = lbOwnerDrawFixed
		ItemHeight = 28
		ItemIndex = 0
    TabStop = True
    TabOrder = 1
		OnDrawItem = ListBox_TypesDrawItem
  end
  object Button_Ok: TButton
    Left = 356
    Top = 4
    Width = 68
    Height = 22
    Caption = 'Ok'
    Anchors = [akTop, akRight]
    OnClick = Button_OkClick
    TabStop = True
    TabOrder = 2
    Default = True
  end
  object Button_Annuler: TButton
    Left = 356
    Top = 32
    Width = 68
    Height = 22
    Caption = 'Annuler'
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 3
		ModalResult = mrCancel
    Cancel = True
  end
  object Button_Aide: TButton
    Caption = 'Aide'
    Left = 356
    Top = 60
    Width = 68
    Height = 22
    Anchors = [akTop, akRight]
    TabStop = True
    TabOrder = 4
    OnClick = Button_AideClick
  end
end

