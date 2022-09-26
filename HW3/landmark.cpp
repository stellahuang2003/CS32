class Landmark {
  public:
    Landmark(string name) : m_name(name) {
        m_color = "yellow";
    };
    virtual ~Landmark() {}
    virtual string color() const { // virtual bc diff for only Hospital
        return m_color;
    }
    string name() const { // non-virtual bc it's the same for all three
        return m_name;
    }
    virtual string icon() const = 0; // pure-virtual bc diff for all three
    
  private:
    string m_name;
    string m_color;
};

class Hotel : public Landmark {
  public:
    Hotel(string name) : Landmark(name) {}
    virtual ~Hotel() {
        cout << "Destroying the hotel " << name() << ".\n";
    }
    virtual string icon() const {
        return "bed";
    }
};

class Restaurant : public Landmark {
  public:
    Restaurant(string name, int size) : Landmark(name) {
        if(size < 40)
            m_icon = "small knife/fork";
        else
            m_icon = "large knife/fork";
    }
    virtual string icon() const {
        return m_icon;
    }
    virtual ~Restaurant() {
        cout << "Destroying the restaurant " << name() << ".\n";
    }
    
  private:
    string m_icon;
};

class Hospital : public Landmark {
  public:
    Hospital(string name) : Landmark(name) {}
    virtual ~Hospital() {
        cout << "Destroying the hospital " << name() << ".\n";
    }
    virtual string icon() const {
        return "H";
    }
    virtual string color() const {
        return "blue";
    }
};
