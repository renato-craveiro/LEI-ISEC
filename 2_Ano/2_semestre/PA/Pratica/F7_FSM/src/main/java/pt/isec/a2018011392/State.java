package pt.isec.a2018011392;

import javax.naming.Context;

public enum State {
   CLOSED, OPEN, LOCKED;

   //Factory: instance method (package-private)
   IState getInstance(Context context, SafeDate data) {
      return switch (this) {
         case CLOSED -> new ClosedState(context,data);
         case OPEN -> new OpenState(context,data);
         case LOCKED -> new LockedState(context, data);
      };
   }

}
