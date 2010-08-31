#ifndef STATICACTION_H_
#define STATICACTION_H_

#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>


/**
 * Perform static action
 */
class StaticAction { 
public:

  enum StaticActionType
  {
      actNone = 0,

      actForwardDribbleRight,
      actForwardDribbleLeft,
      actForwardKickRight,
      actForwardKickLeft,
      actForwardStrongRight,
      actForwardStrongLeft,

      actSideMiniToRightIn,
      actSideMiniToLeftIn,
//      actSideWeakToRightIn,
//      actSideWeakToLeftIn,
//      actSideKickToRightIn,
//      actSideKickToLeftIn,
      actSideStrongToRightIn,
      actSideStrongToLeftIn,
      actSideKickToRightOut,
      actSideKickToLeftOut,

//      actObliqueKickToRight,
//      actObliqueKickToLeft,

      actSpreadOut,
      actGetUpFromFront,
      actGetUpFromBack,
      actStandUp,
      actSitDown,
      actTest,
      numStaticActions // this should always be last!
  };
        


  /**
   * Constructor.
   */
  StaticAction(StaticActionType actionID = actNone);

  /**
   * Destructor.
   */
  virtual ~StaticAction();


  /**
   * Returns the action ID
   *
   * @return action ID
   */
  StaticActionType getActionID() const;

  /**
   * Sets the action ID
   *
   * @param _actionID action ID
   */
  void setActionID(StaticActionType _actionID);

private:

  StaticActionType actionID;

  /**
   * Allow access for Boost serialization
   */
  friend class boost::serialization::access;

  template<class Archive>
  void serialize(Archive & ar, unsigned int version);
};

// Boost doesn't allow this to be placed in the cpp file for some reason.
template<class Archive>
void StaticAction::serialize(Archive & ar, unsigned int version) {

  ar & actionID;
}

#endif /* STATICACTION_H_ */
