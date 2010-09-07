package shape;

public enum Screen {
  // These values should match Log/Log.h
  originalImage (1),
  segmentedImage(2),
  field         (3),
  textConsole   (4);

  Screen(int id) {
    this.id = id;
  }
  
  public int getID() {
    return id;
  }

  private int id;

  public static Screen create(int id) {
    for (Screen screen : Screen.values()) {
      if (screen.id == id) {
        return screen;
      }
    }

    return Screen.field;
  }

}
