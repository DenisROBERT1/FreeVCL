object Form_SaisieRacc: TForm_SaisieRacc
  Left = 207
  Top = 107
  AutoScroll = False
  Caption = 'Définition des raccourcis'
  Width = 706
  Height = 460
  Color = clBtnFace
  Constraints.MinWidth = 706
  Constraints.MinHeight = 460
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnDragOver = FormDragOver
  DesignSize = (
    693
    433)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelFonctions: TLabel
    Left = 6
    Top = 3
    Width = 122
    Height = 13
    Caption = 'Fonctions du programme :'
  end
  object TreeView_Fnct: TTreeView
    Left = 6
    Top = 20
    Width = 211
    Height = 356
    Anchors = [akLeft, akTop, akRight, akBottom]
    DragMode = dmAutomatic
    HideSelection = False
    ShowRoot = True
    Indent = 19
    ReadOnly = True
    TabOrder = 0
    OnSelect = TreeView_FnctSelect
    OnDblClick = TreeView_FnctDblClick
    OnEndDrag = TreeView_FnctEndDrag
    OnStartDrag = TreeView_FnctStartDrag
  end
  object LabelListeRacc: TLabel
    Left = 227
    Top = 3
    Width = 97
    Height = 13
    Anchors = [akTop, akRight]
    Caption = 'Liste des raccourcis:'
  end
  object ListView_Raccourcis: TListView
    Left = 228
    Top = 20
    Width = 324
    Height = 356
    Anchors = [akTop, akRight, akBottom]
    Columns = <
      item
        Caption = 'Commande'
        Width = 149
      end
      item
        Caption = 'Raccourci'
        Width = 154
      end>
    DragMode = dmAutomatic
    HideSelection = False
    ReadOnly = True
    RowSelect = True
    SortType = stBoth
    TabOrder = 1
    ViewStyle = vsReport
    OnChange = ListView_RaccourcisChange
    OnCompare = ListView_RaccourcisCompare
    OnEndDrag = ListView_RaccourcisEndDrag
    OnDragOver = ListView_RaccourcisDragOver
    OnKeyDown = ListView_RaccourcisKeyDown
  end
  object Button_Supprimer: TButton
    Left = 564
    Top = 21
    Width = 127
    Height = 22
    Hint = 'Supprime le raccourci sélectionn'#233
    Anchors = [akTop, akRight]
    Caption = 'Supprimer'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    OnClick = Button_SupprimerClick
  end
  object Button_MAZ: TButton
    Left = 563
    Top = 51
    Width = 128
    Height = 22
    Hint = 'Supprime tous les raccourcis'
    Anchors = [akTop, akRight]
    Caption = 'Mise '#224' zéro'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 3
    OnClick = Button_MAZClick
  end
  object Button_Default: TButton
    Left = 563
    Top = 81
    Width = 128
    Height = 22
    Hint = 'Rétablit les valeurs par défaut'
    Anchors = [akTop, akRight]
    Caption = 'Valeurs par défaut'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 4
    OnClick = Button_DefaultClick
  end
  object GroupBox: TGroupBox
    Left = 563
    Top = 111
    Width = 127
    Height = 141
    Anchors = [akTop, akRight]
    Caption = 'Raccourci'
    TabOrder = 5
    object PaintBox_Alt: TPaintBox
      Left = 12
      Top = 21
      Width = 32
      Height = 32
      Hint = 'Détermine si la touche ALT est enfoncée'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      OnClick = PaintBox_AltClick
      OnPaint = PaintBox_AltPaint
    end
    object PaintBox_Shift: TPaintBox
      Left = 48
      Top = 21
      Width = 32
      Height = 32
      Hint = 'Détermine si la touche SHIFT est enfoncée'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      OnClick = PaintBox_ShiftClick
      OnPaint = PaintBox_ShiftPaint
    end
    object PaintBox_Ctrl: TPaintBox
      Left = 84
      Top = 21
      Width = 32
      Height = 32
      Hint = 'Détermine si la touche CTRL est enfoncée'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      OnClick = PaintBox_CtrlClick
      OnPaint = PaintBox_CtrlPaint
    end
    object ComboBox_ToucheRacc: TComboBox
      Left = 12
      Top = 60
      Width = 103
      Height = 38
      Hint = 'Détermine quelle touche est enfoncée'
      ItemHeight = 32
      Style = csOwnerDrawFixed
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 0
      OnDrawItem = ComboBox_ToucheRaccDrawItem
      OnSelect = RaccourciChange
    end
    object HotKey_SaisieDirecte: THotKey
      Left = 12
      Top = 108
      Width = 103
      Height = 21
      Hint = 'Permet de saisir directement le raccourci'
      Enabled = False
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnChange = HotKey_SaisieDirecteChange
    end
  end
  object Button_Ok: TButton
    Left = 209
    Top = 391
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    TabOrder = 6
    Default = True
    OnClick = Button_OkClick
  end
  object Button_Annuler: TButton
    Left = 305
    Top = 391
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Annuler'
    ModalResult = mrCancel
    TabOrder = 7
    Cancel = True
  end
  object Button_Aide: TButton
    Left = 401
    Top = 391
    Width = 73
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Aide'
    TabOrder = 8
    OnClick = Button_AideClick
  end
end
