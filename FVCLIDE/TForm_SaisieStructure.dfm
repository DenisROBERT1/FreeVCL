object Form_SaisieStructure: TForm_SaisieStructure
  Left = 301
  Top = 223
  Width = 517
  Height = 436
  Caption = 'Création automatique d'#39'unité'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    509
    312)
  PixelsPerInch = 96
  TextHeight = 13
  object Label_Repertoire: TLabel
    Left = 6
    Top = 8
    Width = 74
    Height = 13
    Caption = 'Destination :'
  end
  object Edit_Repertoire: TEdit
    Left = 84
    Top = 4
    Width = 330
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    ParentShowHint = False
    TabOrder = 0
  end
  object Button_Parcourir: TButton
    Left = 420
    Top = 4
    Width = 84
    Height = 21
    Anchors = [akTop, akRight]
    Caption = 'Parcourir'
    TabOrder = 1
    OnClick = Button_ParcourirClick
  end
  object Label_Unite: TLabel
    Left = 6
    Top = 34
    Width = 70
    Height = 13
    Caption = 'Nom unité :'
  end
  object Label_T: TLabel
    Left = 76
    Top = 34
    Width = 6
    Height = 13
    Alignment = taRightJustify
    Caption = 'T'
  end
  object Edit_Unite: TEdit
    Left = 84
    Top = 30
    Width = 144
    Height = 21
    AutoSize = False
    TabOrder = 2
    Anchors = [akLeft, akTop, akRight]
		Text = 'Form_'
  end
  object Label_Commentaire: TLabel
    Left = 232
    Top = 34
    Width = 200
    Height = 13
    Anchors = [akTop, akRight]
    Caption = '(Nom de la classe = T + nom unité)'
  end
  object Label_Caption: TLabel
    Left = 6
    Top = 60
    Width = 70
    Height = 13
    Caption = 'Titre fenêtre :'
  end
  object Edit_Caption: TEdit
    Left = 84
    Top = 56
    Width = 300
    Height = 21
    TabOrder = 3
    Anchors = [akLeft, akTop, akRight]
  end
  object Label_Struct: TLabel
    Left = 6
    Top = 84
    Width = 500
    Height = 44
    Caption = 'Structure à saisir. par exemple:'#10'char Nom[128]; // Nom'#10'char Prenom[128]; // Prénom'
  end
  object Memo_Struct: TMemo
    Left = 6
    Top = 130
    Width = 500
    Height = 220
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
  end
  object Button_Apercu: TButton
    Left = 104
    Top = 366
    Width = 80
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Aperçu'
    TabOrder = 5
    OnClick = Button_ApercuClick
  end
  object Button_Creer: TButton
    Left = 204
    Top = 366
    Width = 80
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Créer'
    TabOrder = 6
    Default = True
    OnClick = Button_CreerClick
  end
  object Button_Annuler: TButton
    Left = 304
    Top = 366
    Width = 80
    Height = 24
    Anchors = [akLeft, akBottom]
    Caption = 'Annuler'
    TabOrder = 7
    Cancel = True
    ModalResult = mrCancel
  end
end
