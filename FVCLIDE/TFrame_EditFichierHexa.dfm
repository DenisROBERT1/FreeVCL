object Frame_EditFichierHexa: TFrame_EditFichierHexa
  Left = 282
  Top = 133
  Width = 460
  Height = 383
  Color = clBtnFace
  Visible = True
  OnShow = Frame_EditFichierHexaShow
  object Panel: TPanel
    Left = 6
    Top = 6
    Width = 439
    Height = 325
    Align = alClient
    TabStop = True
    TabOrder = 0
    BevelOuter = bvLowered
    object HexaEditor: THexaEditor
      Left = 0
      Top = 0
      Width = 439
      Height = 325
      Align = alClient
      TabStop = True
      TabOrder = 0
      DragAcceptFiles = True
      OnContextPopup = HexaEditorContextPopup
      OnDropFile = HexaEditorDropFile
    end
  end
  object PopupMenu_Context: TPopupMenu
  end
end
