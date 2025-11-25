#ifndef CUBOID_HH
#define CUBOID_HH

#include "AbstractMobileObj.hh"
#include "Vector3D.hh"
#include <string>

class Cuboid : public AbstractMobileObj {
  private:
    std::string _name;  
    Vector3D _shift;
    Vector3D _trans_m;     
    Vector3D _scale;        
    Vector3D _rgb;          
    Vector3D _rotxyz_deg;

  public:
    Cuboid(){
        _shift[0] = 0.0; _shift[1] = 0.0; _shift[2] = 0.0;
        _trans_m[0] = 0.0; _trans_m[1] = 0.0; _trans_m[2] = 0.0;
        _scale[0] = 1.0; _scale[1] = 1.0; _scale[2] = 1.0;
        _rgb[0] = 128; _rgb[1] = 128; _rgb[2] = 128;
        _rotxyz_deg[0] = 0; _rotxyz_deg[1] = 0; _rotxyz_deg[2] = 0;
    }

    virtual double GetAng_Roll_deg() const override  {return _rotxyz_deg[0];}
    virtual double GetAng_Pitch_deg() const override {return _rotxyz_deg[1];}
    virtual double GetAng_Yaw_deg() const override   {return _rotxyz_deg[2];}

    virtual void SetAng_Roll_deg(double ang) override  {_rotxyz_deg[0] = ang;}
    virtual void SetAng_Pitch_deg(double ang) override {_rotxyz_deg[1] = ang;}
    virtual void SetAng_Yaw_deg(double ang) override   {_rotxyz_deg[2] = ang;}

    virtual const std::string & GetName() const override {return _name;}
    virtual void SetName(const char* sName) override {_name = sName;}
    
    const Vector3D& GetShift() const {return _shift;}
    void SetShift(const Vector3D& shift) {_shift = shift;}
    
    const Vector3D& GetScale() const {return _scale;}
    void SetScale(const Vector3D& scale) {_scale = scale;}
    
    const Vector3D& GetRGB() const {return _rgb;}
    void SetRGB(const Vector3D& rgb) {_rgb = rgb;}

    const Vector3D & GetPosition_m() const {return _trans_m;}
    void SetPosition_m(const Vector3D & rTrans) {_trans_m = rTrans;}
};

#endif